#ifndef HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_ECHO_COMPONENT_H_INCLUDED
#define HEADER_GAME_ENGINE_SRC_INC_COMPONENTS_ECHO_COMPONENT_H_INCLUDED
#include "Component.h"

//! Calls its input functor whenever updated
struct MimicC : ProcessorComponent {
    std::function<void(float)> functor;

    template <typename F>
    MimicC(F&& f) : functor{std::forward<F>(f)} {};

    virtual void update(float dt) override
    {
        functor(dt);
    }
} /*struct MimicC*/;

#endif
