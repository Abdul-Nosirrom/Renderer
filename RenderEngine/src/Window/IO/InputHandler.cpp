//
// Created by Abdulrahmen on 10/10/2024.
//

#include "InputHandler.h"

InputHandler& InputHandler::Get()
{
    static InputHandler instance;
    return instance;
}
