#include "xdkbutton.h"
#include "button.h"
#include "XdkBoardHandle.h"

int EnableButton(BUTTON_handle_tp* buttonHandle);
int SetButtonCallback(BUTTON_handle_tp* buttonHandle, buttonHandler handler);

static BUTTON_handle_tp ButtonOneHandle = (BUTTON_handle_tp) NULL;

int ButtonOneInit(buttonHandler handler)
{
    int ret = -1;
    ButtonOneHandle = BUTTON_create(gpioButton1_Handle, GPIO_STATE_OFF);

    printf("Button handle %x\n", (int)ButtonOneHandle);
    if(NULL != ButtonOneHandle)
    {
        if(0 == EnableButton(&ButtonOneHandle))
        {
            ret = SetButtonCallback(&ButtonOneHandle, handler);
        }
    }

    if(-1 == ret)
    {
        printf("Button creation FAILED!\n\r");
    }

    return ret;
}

int EnableButton(BUTTON_handle_tp* buttonHandle)
{
    int ret = 0;
    BUTTON_errorTypes_t buttonReturn = BUTTON_ERROR_INVALID_PARAMETER;
    buttonReturn = BUTTON_enable(*buttonHandle);
    if(BUTTON_ERROR_OK != buttonReturn)
    {
        printf("Enabling Button failed with status = %d!\n", (int)buttonReturn);
        ret = -1;
    }
    return ret;
}

int SetButtonCallback(BUTTON_handle_tp* buttonHandle, buttonHandler handler)
{
    int ret = 0;
    BUTTON_errorTypes_t buttonReturn = BUTTON_ERROR_INVALID_PARAMETER;
    buttonReturn = BUTTON_setCallback(*buttonHandle, handler, BUTTONONE);
    if(BUTTON_ERROR_OK != buttonReturn)
    {
        printf("Setting callback for button failed with status = %d!\n", (int)buttonReturn);
        ret = -1;
    }
    return ret;
}

int IsButtonPressed(void* handle)
{
    return BUTTON_isPressed(handle);
}

int IsButtonReleased(void* handle)
{
    return BUTTON_isReleased(handle);
}
