/*
 *  MIT License
 *  
 *  Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include <vector>

#include "Singleton.h"
#include "LibLoader.h"
#include "Log.h"
#include "IAudio.h"
#include "IRenderer.h"
#include "IGamepadManager.h"
#include "IKeyboard.h"
#include "IMouse.h"

struct Settings {
    struct Server {
        std::string name;
        std::string ip;
        std::string port;
    };
    
    std::string username                = "Default1";
    int   width                         = 1280;
    int   height                        = 720;
    ExoRenderer::WindowMode  windowMode = ExoRenderer::WindowMode::WINDOWED;
    bool  vsync                         = false;
    bool  fps                           = false;
    int   globalVolume                  = 100;
    std::string language                = "en";
    uint8_t threads                     = 4;
    std::vector<Server> servers;
    std::string libGraphic              = "libRendererSDLOpenGL";
    std::string libAudio                = "libOpenAL";
};

class SManager : public Singleton<SManager>
{
public:
    // Methods
    void loadRendererLib(void);
    void unloadRendererLib(void);
    
    void loadAudioLib(void);
    void unloadAudioLib(void);
    
    void addAudioSource(const std::shared_ptr<ISound> &sound);
    void updateAudioSources();
    
    void loadSettings(void);
    void updateSettings(void);
    
    // Getters
    ExoRenderer::IRenderer* getRenderer(void);
    
    ExoRenderer::ICursor* getCursor(void);
    ExoRenderer::IKeyboard* getKeyboard(void);
    ExoRenderer::IMouse* getMouse(void);
    ExoRenderer::IGamepadManager* getGamepadManager(void);
    
    IAudio* getAudio(void);
    
    // Setters
    void setCursor(ExoRenderer::ICursor* cursor);
    void setKeyboard(ExoRenderer::IKeyboard* keyboard);
    void setMouse(ExoRenderer::IMouse* mouse);
    void setGamepadManager(ExoRenderer::IGamepadManager* gamepad);
public:
    Settings settings;
private:
    friend class Singleton<SManager>;
    
    SManager(void);
    ~SManager(void);

    LibLoader<ExoRenderer::IRenderer>*   _pRendererLibrary;
    LibLoader<IAudio>*      _pAudioLibrary;
    
    ExoRenderer::ICursor*           _pCursor;
    ExoRenderer::IKeyboard*         _pKeyboard;
    ExoRenderer::IMouse*            _pMouse;
    ExoRenderer::IGamepadManager*   _pGamepad;
    
    std::vector<ISource*> _globalSources;
};
