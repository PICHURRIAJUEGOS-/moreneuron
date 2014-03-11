// #define COCOS2D_DEBUG   1

#include "TextInput.h"

/**
 *Esta libreria permite crear 2 tipos de entrada de texto.
 * 1) new TextFieldTTFDefault, para entrada de texto sencillo
 * 2) new TextFieldTTFAction, para entrada de texto con efecto
 */
//////////////////////////////////////////////////////////////////////////
// local function
//////////////////////////////////////////////////////////////////////////

#define FONT_NAME                       "Thonburi"
#define FONT_SIZE                       56

static int testIdx = -1; 

static Rect getRect(Node * node)
{
    Rect rc;
    rc.origin = node->getPosition();
    rc.size = node->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}

//////////////////////////////////////////////////////////////////////////
// implement KeyboardNotificationLayer
//////////////////////////////////////////////////////////////////////////

KeyboardNotificationLayer::KeyboardNotificationLayer()
  : _trackNode(0), onEditingEnd(nullptr)
{
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void KeyboardNotificationLayer::keyboardWillShow(IMEKeyboardNotificationInfo& info)
{
    CCLOG("TextInputTest:keyboardWillShowAt(origin:%f,%f, size:%f,%f)",
        info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);

    if (! _trackNode)
    {
        return;
    }

    auto rectTracked = getRect(_trackNode);
    CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
        rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);

    // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
    if (! rectTracked.intersectsRect(info.end))
    {
        return;
    }

    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);

    // move all the children node of KeyboardNotificationLayer
    auto& children = getChildren();
    Node * node = 0;
    int count = children.size();
    Point pos;
    for (int i = 0; i < count; ++i)
    {
      node = (Node*)children.at(i);
      if(!node) continue;
      pos = node->getPosition();
      pos.y += adjustVert;
      node->setPosition(pos);
    }
}

// Layer function

bool KeyboardNotificationLayer::onTouchBegan(Touch  *touch, Event  *event)
{
    CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
    _beginPos = touch->getLocation();    
    return true;
}

void KeyboardNotificationLayer::onTouchEnded(Touch  *touch, Event  *event)
{
    if (! _trackNode)
    {
        return;
    }
    
    auto endPos = touch->getLocation();    

    float delta = 5.0f;
    if (::abs(endPos.x - _beginPos.x) > delta
        || ::abs(endPos.y - _beginPos.y) > delta)
    {
        // not click
        _beginPos.x = _beginPos.y = -1;
        return;
    }

    // decide the trackNode is clicked.
    Rect rect;
    auto point = convertTouchToNodeSpaceAR(touch);
    CCLOG("KeyboardNotificationLayer:clickedAt(%f,%f)", point.x, point.y);

    rect = getRect(_trackNode);
    CCLOG("KeyboardNotificationLayer:TrackNode at(origin:%f,%f, size:%f,%f)",
        rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);

    this->onClickTrackNode(rect.containsPoint(point));
    if(!rect.containsPoint(point)){
      if(onEditingEnd != nullptr){
	auto pTextField = (TextFieldTTF*)_trackNode;
	onEditingEnd(pTextField->getString());
      }
    }
    CCLOG("----------------------------------");
}

void KeyboardNotificationLayer::onTouchCancelled(Touch *touch, Event *event)
{
  CCLOG("KeyboardNotificationLayer:onTouchCancelled");
  this->onTouchEnded(touch, event);
}

//////////////////////////////////////////////////////////////////////////
// implement TextFieldTTFDefault
//////////////////////////////////////////////////////////////////////////

std::string TextFieldTTFDefault::subtitle()
{
    return "TextFieldTTF with default behavior";
}


void TextFieldTTFDefault::onClickTrackNode(bool bClicked)
{
    auto pTextField = (TextFieldTTF*)_trackNode;
    if(!_trackNode) return;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("TextFieldTTFDefault:TextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("TextFieldTTFDefault:TextFieldTTF detachWithIME");

        pTextField->detachWithIME();
    }
}

void TextFieldTTFDefault::onEnter()
{
    KeyboardNotificationLayer::onEnter();

    // add TextFieldTTF
    auto s = Director::getInstance()->getWinSize();

    auto pTextField = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
        FONT_NAME,
        FONT_SIZE);
    addChild(pTextField);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)    
    // on android, TextFieldTTF cannot auto adjust its position when soft-keyboard pop up
    // so we had to set a higher position to make it visable
    pTextField->setPosition(Point(s.width / 2, s.height/2 + 50));
#else
    pTextField->setPosition(Point(s.width / 2, s.height / 2));
#endif

    _trackNode = pTextField;
}

//////////////////////////////////////////////////////////////////////////
// implement TextFieldTTFAction
//////////////////////////////////////////////////////////////////////////

std::string TextFieldTTFAction::subtitle()
{
    return "CCTextFieldTTF with action and char limit test";
}

void TextFieldTTFAction::onClickTrackNode(bool bClicked)
{
    auto pTextField = (TextFieldTTF*)_trackNode;
    if(!pTextField) return;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("TextFieldTTFAction:TextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("TextFieldTTFAction:TextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}

void TextFieldTTFAction::onEnter()
{
    KeyboardNotificationLayer::onEnter();

    _charLimit = 12;

    _textFieldAction = RepeatForever::create(
        Sequence::create(
            FadeOut::create(0.25),
            FadeIn::create(0.25),
            NULL
        ));
    _textFieldAction->retain();
    _action = false;

    // add TextFieldTTF
    auto s = Director::getInstance()->getWinSize();

    _textField = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
        FONT_NAME,
        FONT_SIZE);
    addChild(_textField);

    _textField->setDelegate(this);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)    
    // on android, TextFieldTTF cannot auto adjust its position when soft-keyboard pop up
    // so we had to set a higher position
    _textField->setPosition(Point(s.width / 2, s.height/2 + 50));
#else
    _textField->setPosition(Point(s.width / 2, s.height / 2));
#endif

    _trackNode = _textField;
}

void TextFieldTTFAction::onExit()
{
    KeyboardNotificationLayer::onExit();
    _textFieldAction->release();
}

// TextFieldDelegate protocol
bool TextFieldTTFAction::onTextFieldAttachWithIME(TextFieldTTF * sender)
{
    if (! _action)
    {
        _textField->runAction(_textFieldAction);
        _action = true;
    }
    return false;
}

bool TextFieldTTFAction::onTextFieldDetachWithIME(TextFieldTTF * sender)
{
    if (_action)
    {
        _textField->stopAction(_textFieldAction);
        _textField->setOpacity(255);
        _action = false;
    }
    return false;
}

bool TextFieldTTFAction::onTextFieldInsertText(TextFieldTTF * sender, const char * text, int nLen)
{
    // if insert enter, treat as default to detach with ime
    if ('\n' == *text)
    {
        return false;
    }
    
    // if the textfield's char count more than _charLimit, doesn't insert text anymore.
    if (sender->getCharCount() >= _charLimit)
    {
        return true;
    }

    // create a insert text sprite and do some action
    auto label = LabelTTF::create(text, FONT_NAME, FONT_SIZE);
    this->addChild(label);
    Color3B color(226, 121, 7);
    label->setColor(color);

    // move the sprite from top to position
    auto endPos = sender->getPosition();
    if (sender->getCharCount())
    {
        endPos.x += sender->getContentSize().width / 2;
    }
    auto inputTextSize = label->getContentSize();
    Point beginPos(endPos.x, Director::getInstance()->getWinSize().height - inputTextSize.height * 2); 

    float duration = 0.5;
    label->setPosition(beginPos);
    label->setScale(8);

    auto seq = Sequence::create(
        Spawn::create(
            MoveTo::create(duration, endPos),
            ScaleTo::create(duration, 1),
            FadeOut::create(duration),
            NULL),
        CallFuncN::create(CC_CALLBACK_1(TextFieldTTFAction::callbackRemoveNodeWhenDidAction, this)),
        NULL);
    label->runAction(seq);
    return false;
}

bool TextFieldTTFAction::onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, int nLen)
{
    // create a delete text sprite and do some action
    auto label = LabelTTF::create(delText, FONT_NAME, FONT_SIZE);
    this->addChild(label);

    // move the sprite to fly out
    auto beginPos = sender->getPosition();
    auto textfieldSize = sender->getContentSize();
    auto labelSize = label->getContentSize();
    beginPos.x += (textfieldSize.width - labelSize.width) / 2.0f;
    
    auto winSize = Director::getInstance()->getWinSize();
    Point endPos(- winSize.width / 4.0f, winSize.height * (0.5 + (float)rand() / (2.0f * RAND_MAX)));

    float duration = 1;
    float rotateDuration = 0.2f;
    int repeatTime = 5; 
    label->setPosition(beginPos);

    auto seq = Sequence::create(
        Spawn::create(
            MoveTo::create(duration, endPos),
            Repeat::create(
                RotateBy::create(rotateDuration, (rand()%2) ? 360 : -360),
                repeatTime),
            FadeOut::create(duration),
        NULL),
        CallFuncN::create(CC_CALLBACK_1(TextFieldTTFAction::callbackRemoveNodeWhenDidAction, this)),
        NULL);
    label->runAction(seq);
    return false;
}

bool TextFieldTTFAction::onDraw(TextFieldTTF * sender)
{
    return false;
}

void TextFieldTTFAction::callbackRemoveNodeWhenDidAction(Node * node)
{
    this->removeChild(node, true);
}

