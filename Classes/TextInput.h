/**
 *This is a mod from text_input_text
 */
#ifndef __TEXT_INPUT_TEST_H__
#define __TEXT_INPUT_TEST_H__

#include "cocos2d.h"
#include <string>
//#include "../testBasic.h"
//#include "../BaseTest.h"
USING_NS_CC;
class KeyboardNotificationLayer;


//////////////////////////////////////////////////////////////////////////
// KeyboardNotificationLayer for test IME keyboard notification.
//////////////////////////////////////////////////////////////////////////

class KeyboardNotificationLayer : public Layer, public IMEDelegate
{
public:
    KeyboardNotificationLayer();

    virtual std::string subtitle() = 0;
    virtual void onClickTrackNode(bool bClicked) = 0;
    virtual void keyboardWillShow(IMEKeyboardNotificationInfo& info);

    // Layer
    bool onTouchBegan(Touch  *touch, Event  *event);
    void onTouchEnded(Touch  *touch, Event  *event);
    void onTouchCancelled(Touch *touch, Event *event);
    std::function<void(const std::string)> onEditingEnd;
protected:
    Node * _trackNode;
    Point  _beginPos;

};

//////////////////////////////////////////////////////////////////////////
// TextFieldTTFDefault for test TextFieldTTF default behavior.
//////////////////////////////////////////////////////////////////////////

class TextFieldTTFDefault : public KeyboardNotificationLayer
{
public:
    // KeyboardNotificationLayer
    virtual std::string subtitle();
    virtual void onClickTrackNode(bool bClicked);

    // Layer
    virtual void onEnter();
};

//////////////////////////////////////////////////////////////////////////
// TextFieldTTFAction
//////////////////////////////////////////////////////////////////////////

class TextFieldTTFAction : public KeyboardNotificationLayer, public TextFieldDelegate
{
    TextFieldTTF *    _textField;
    Action *          _textFieldAction;
    bool                _action;
    int                 _charLimit;       // the textfield max char limit

public:
    void callbackRemoveNodeWhenDidAction(Node * node);

    // KeyboardNotificationLayer
    virtual std::string subtitle();
    virtual void onClickTrackNode(bool bClicked);

    // Layer
    virtual void onEnter();
    virtual void onExit();

    // TextFieldDelegate
    virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender);
    virtual bool onTextFieldDetachWithIME(TextFieldTTF * sender);
    virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, int nLen);
    virtual bool onDraw(TextFieldTTF * sender);
};

KeyboardNotificationLayer* createTextInputTest(int nIndex);
#endif    // __TEXT_INPUT_TEST_H__
