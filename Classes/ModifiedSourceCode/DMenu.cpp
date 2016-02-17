/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "ModifiedSourceCode/DMenu.h"
#include "base/CCDirector.h"
#include "CCApplication.h"
#include "base/CCTouch.h"
#include "CCStdC.h"
#include "base/CCEventListenerTouch.h"
#include "deprecated/CCString.h"

#include <vector>
#include <stdarg.h>

using namespace std;

NS_CC_BEGIN

enum
{
    kDefaultPadding =  5,
};

//
//CCMenu
//

DMenu::~DMenu()
{
    CCLOGINFO("In the destructor of Menu. %p", this);
}


Menu* DMenu::create()
{
    return DMenu::create(nullptr, nullptr);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
Menu * Menu::variadicCreate(MenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    Menu *ret = Menu::createWithItems(item, args);
    
    va_end(args);
    
    return ret;
}
#else


Menu * DMenu::create(MenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    Menu *ret = DMenu::createWithItems(item, args);
    
    va_end(args);
    
    return ret;
}
#endif


Menu* DMenu::createWithArray(const Vector<MenuItem*>& arrayOfItems)
{
    auto ret = new DMenu();
    if (ret && ret->initWithArray(arrayOfItems))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

Menu* DMenu::createWithItems(MenuItem* item, va_list args)
{
    Vector<MenuItem*> items;
    if( item )
    {
        items.pushBack(item);
        MenuItem *i = va_arg(args, MenuItem*);
        while(i)
        {
            items.pushBack(i);
            i = va_arg(args, MenuItem*);
        }
    }
    
    return DMenu::createWithArray(items);
}

Menu* DMenu::createWithItem(MenuItem* item)
{
    return DMenu::create(item, nullptr);
}

bool DMenu::init()
{
    return initWithArray(Vector<MenuItem*>());
}

bool DMenu::initWithArray(const Vector<MenuItem*>& arrayOfItems)
{
    if (Layer::init())
    {
        _enabled = true;
        // menu in the center of the screen
        Size s = Director::getInstance()->getWinSize();
        
        this->ignoreAnchorPointForPosition(true);
        setAnchorPoint(Vec2(0.5f, 0.5f));
        this->setContentSize(s);
        
        setPosition(Vec2(s.width/2, s.height/2));
        
        int z=0;
        
        for (auto& item : arrayOfItems)
        {
            this->addChild(item, z);
            z++;
        }
        
        _selectedItem = nullptr;
        _state = DMenu::State::WAITING;
        
        // enable cascade color and opacity on menus
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
        
        
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(false);
        CCLOG("swallow parameter is %d",m_bSwallowsTouches);
        touchListener->onTouchBegan = CC_CALLBACK_2(Menu::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(Menu::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(Menu::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(Menu::onTouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
        return true;
    }
    return false;
}
//用于不在scrollview外部点击menu
void DMenu::setTouchlimit(cocos2d::Node *node)
{
    m_szTouchLimitNode=node;
    m_bTouchLimit=true;
}
bool DMenu::isInTouchLimit(Touch* touch)
{
    if(m_bTouchLimit)
    {
        Vec2 touchLocation = touch->getLocation();
        
        Vec2 local = m_szTouchLimitNode->convertToNodeSpace(touchLocation);
        Rect r = m_szTouchLimitNode->getBoundingBox();
        r.origin = Vec2::ZERO;
        
        if (!r.containsPoint(local))
        {
            return true;
        }
    }
    return false;
}
void DMenu::setSwallowTouches(bool needSwallows){
    m_bSwallowsTouches = needSwallows;
}
/*
 * override add:
 */
void DMenu::addChild(Node * child)
{
    Layer::addChild(child);
}

void DMenu::addChild(Node * child, int zOrder)
{
    Layer::addChild(child, zOrder);
}

void DMenu::addChild(Node * child, int zOrder, int tag)
{
    CCASSERT( dynamic_cast<MenuItem*>(child) != nullptr, "Menu only supports MenuItem objects as children");
    Layer::addChild(child, zOrder, tag);
}

void DMenu::onEnter()
{
    Layer::onEnter();
}

void DMenu::onExit()
{
    if (_state == DMenu::State::TRACKING_TOUCH)
    {
        if (_selectedItem)
        {
            _selectedItem->unselected();
            _selectedItem = nullptr;
        }
        
        _state = DMenu::State::WAITING;
    }
    
    Layer::onExit();
}

void DMenu::removeChild(Node* child, bool cleanup)
{
    MenuItem *menuItem = dynamic_cast<MenuItem*>(child);
    CCASSERT(menuItem != nullptr, "Menu only supports MenuItem objects as children");
    
    if (_selectedItem == menuItem)
    {
        _selectedItem = nullptr;
    }
    
    Node::removeChild(child, cleanup);
}

//Menu - Events

bool DMenu::onTouchBegan(Touch* touch, Event* event)
{
    isMoved = false;
    if (_state != DMenu::State::WAITING || ! _visible || !_enabled)
    {
        return false;
    }
    
    for (Node *c = this->_parent; c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    //-----------newly added
    if(isInTouchLimit(touch))//用于检测是否在scrollview外部
    {
        return false;
    }
    if(!m_bSwallowsTouches){//用于避免用menu拖动scrollview时，touchOnEnded还是menu并触发消息
        m_touchBeginWorldPos = convertToWorldSpace(getPosition());
    }
    //-----------
    
    _selectedItem = this->getItemForTouch(touch);
    if (_selectedItem)
    {
        _state = DMenu::State::TRACKING_TOUCH;
        _selectedItem->selected();
        
        return true;
    }
    
    return false;
}

void DMenu::onTouchEnded(Touch* touch, Event* event)
{
    CCASSERT(_state == DMenu::State::TRACKING_TOUCH, "[Menu ccTouchEnded] -- invalid state");
    this->retain();
    if (_selectedItem)
    {
        _selectedItem->unselected();
        //
        do{
            if(!m_bSwallowsTouches){
                CCLOG("检查ended位置");
                //TO DO....目前仅限于初始和最终位置不一样-》不触发touch，以后搞成移动就不触发touch
                auto newWorldPos = convertToWorldSpace(getPosition());
                const static float kMenuMinMove = 2;
                if (fabs(newWorldPos.x - m_touchBeginWorldPos.x)>kMenuMinMove || fabs(newWorldPos.y-m_touchBeginWorldPos.y)>kMenuMinMove) {
                    break;
                }
            }
            if(isMoved)//如果移动，取消激活按钮
                break;
            _selectedItem->activate();
        }while (false);//TO DO....
    }
    _state = DMenu::State::WAITING;
    this->release();
}

void DMenu::onTouchCancelled(Touch* touch, Event* event)
{
    CCASSERT(_state == DMenu::State::TRACKING_TOUCH, "[Menu ccTouchCancelled] -- invalid state");
    this->retain();
    if (_selectedItem)
    {
        _selectedItem->unselected();
    }
    _state = DMenu::State::WAITING;
    this->release();
}

void DMenu::onTouchMoved(Touch* touch, Event* event)
{
    CCASSERT(_state == DMenu::State::TRACKING_TOUCH, "[Menu ccTouchMoved] -- invalid state");
    MenuItem *currentItem = this->getItemForTouch(touch);
    if (currentItem != _selectedItem)
    {
        if (_selectedItem)
        {
            _selectedItem->unselected();
        }
        _selectedItem = currentItem;
        if (_selectedItem)
        {
            _selectedItem->selected();
        }
    }
    isMoved = true;
}

//Menu - Alignment
void DMenu::alignItemsVertically()
{
    this->alignItemsVerticallyWithPadding(kDefaultPadding);
}

void DMenu::alignItemsVerticallyWithPadding(float padding)
{
    float height = -padding;
    
    for(const auto &child : _children)
        height += child->getContentSize().height * child->getScaleY() + padding;
    
    float y = height / 2.0f;
    
    for(const auto &child : _children) {
        child->setPosition(Vec2(0, y - child->getContentSize().height * child->getScaleY() / 2.0f));
        y -= child->getContentSize().height * child->getScaleY() + padding;
    }
}

void DMenu::alignItemsHorizontally(void)
{
    this->alignItemsHorizontallyWithPadding(kDefaultPadding);
}

void DMenu::alignItemsHorizontallyWithPadding(float padding)
{
    float width = -padding;
    for(const auto &child : _children)
        width += child->getContentSize().width * child->getScaleX() + padding;
    
    float x = -width / 2.0f;
    
    for(const auto &child : _children) {
        child->setPosition(Vec2(x + child->getContentSize().width * child->getScaleX() / 2.0f, 0));
        x += child->getContentSize().width * child->getScaleX() + padding;
    }
}

void DMenu::alignItemsInColumns(int columns, ...)
{
    va_list args;
    va_start(args, columns);
    
    this->alignItemsInColumns(columns, args);
    
    va_end(args);
}

void DMenu::alignItemsInColumns(int columns, va_list args)
{
    CCASSERT(columns >= 0, "Columns must be >= 0");
    ValueVector rows;
    while (columns)
    {
        rows.push_back(Value(columns));
        columns = va_arg(args, int);
    }
    alignItemsInColumnsWithArray(rows);
}

void DMenu::alignItemsInColumnsWithArray(const ValueVector& rows)
{
    int height = -5;
    size_t row = 0;
    int rowHeight = 0;
    int columnsOccupied = 0;
    int rowColumns = 0;
    
    for(const auto &child : _children) {
        CCASSERT(row < rows.size(), "");
        
        rowColumns = rows[row].asInt();
        // can not have zero columns on a row
        CCASSERT(rowColumns, "");
        
        float tmp = child->getContentSize().height;
        rowHeight = (unsigned int)((rowHeight >= tmp || isnan(tmp)) ? rowHeight : tmp);
        
        ++columnsOccupied;
        if (columnsOccupied >= rowColumns)
        {
            height += rowHeight + 5;
            
            columnsOccupied = 0;
            rowHeight = 0;
            ++row;
        }
    }
    
    // check if too many rows/columns for available menu items
    CCASSERT(! columnsOccupied, "");
    
    Size winSize = Director::getInstance()->getWinSize();
    
    row = 0;
    rowHeight = 0;
    rowColumns = 0;
    float w = 0.0;
    float x = 0.0;
    float y = (float)(height / 2);
    
    for(const auto &child : _children) {
        if (rowColumns == 0)
        {
            rowColumns = rows[row].asInt();
            w = winSize.width / (1 + rowColumns);
            x = w;
        }
        
        float tmp = child->getContentSize().height;
        rowHeight = (unsigned int)((rowHeight >= tmp || isnan(tmp)) ? rowHeight : tmp);
        
        child->setPosition(Vec2(x - winSize.width / 2,
                                y - child->getContentSize().height / 2));
        
        x += w;
        ++columnsOccupied;
        
        if (columnsOccupied >= rowColumns)
        {
            y -= rowHeight + 5;
            
            columnsOccupied = 0;
            rowColumns = 0;
            rowHeight = 0;
            ++row;
        }
    }
}

void DMenu::alignItemsInRows(int rows, ...)
{
    va_list args;
    va_start(args, rows);
    
    this->alignItemsInRows(rows, args);
    
    va_end(args);
}

void DMenu::alignItemsInRows(int rows, va_list args)
{
    ValueVector array;
    while (rows)
    {
        array.push_back(Value(rows));
        rows = va_arg(args, int);
    }
    alignItemsInRowsWithArray(array);
}

void DMenu::alignItemsInRowsWithArray(const ValueVector& columns)
{
    vector<int> columnWidths;
    vector<int> columnHeights;
    
    int width = -10;
    int columnHeight = -5;
    size_t column = 0;
    int columnWidth = 0;
    int rowsOccupied = 0;
    int columnRows;
    
    for(const auto &child : _children) {
        // check if too many menu items for the amount of rows/columns
        CCASSERT(column < columns.size(), "");
        
        columnRows = columns[column].asInt();
        // can't have zero rows on a column
        CCASSERT(columnRows, "");
        
        // columnWidth = fmaxf(columnWidth, [item contentSize].width);
        float tmp = child->getContentSize().width;
        columnWidth = (unsigned int)((columnWidth >= tmp || isnan(tmp)) ? columnWidth : tmp);
        
        columnHeight += (int)(child->getContentSize().height + 5);
        ++rowsOccupied;
        
        if (rowsOccupied >= columnRows)
        {
            columnWidths.push_back(columnWidth);
            columnHeights.push_back(columnHeight);
            width += columnWidth + 10;
            
            rowsOccupied = 0;
            columnWidth = 0;
            columnHeight = -5;
            ++column;
        }
    }
    
    // check if too many rows/columns for available menu items.
    CCASSERT(! rowsOccupied, "");
    
    Size winSize = Director::getInstance()->getWinSize();
    
    column = 0;
    columnWidth = 0;
    columnRows = 0;
    float x = (float)(-width / 2);
    float y = 0.0;
    
    for(const auto &child : _children) {
        if (columnRows == 0)
        {
            columnRows = columns[column].asInt();
            y = (float) columnHeights[column];
        }
        
        // columnWidth = fmaxf(columnWidth, [item contentSize].width);
        float tmp = child->getContentSize().width;
        columnWidth = (unsigned int)((columnWidth >= tmp || isnan(tmp)) ? columnWidth : tmp);
        
        child->setPosition(Vec2(x + columnWidths[column] / 2,
                                y - winSize.height / 2));
        
        y -= child->getContentSize().height + 10;
        ++rowsOccupied;
        
        if (rowsOccupied >= columnRows)
        {
            x += columnWidth + 5;
            rowsOccupied = 0;
            columnRows = 0;
            columnWidth = 0;
            ++column;
        }
    }
}

MenuItem* DMenu::getItemForTouch(Touch *touch)
{
    Vec2 touchLocation = touch->getLocation();
    
    if (!_children.empty())
    {
        for (auto iter = _children.crbegin(); iter != _children.crend(); ++iter)
        {
            MenuItem* child = dynamic_cast<MenuItem*>(*iter);
            if (child && child->isVisible() && child->isEnabled())
            {
                Vec2 local = child->convertToNodeSpace(touchLocation);
                Rect r = child->rect();
                r.origin = Vec2::ZERO;
                
                if (r.containsPoint(local))
                {
                    return child;
                }
            }
        }
    }
    
    return nullptr;
}

std::string DMenu::getDescription() const
{
    return StringUtils::format("<Menu | Tag = %d>", _tag);
}

NS_CC_END
