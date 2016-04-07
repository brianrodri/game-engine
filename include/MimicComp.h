#pragma once
#include "Component.h"
#include <functional>

//! Calls its input function whenever updated
struct MimicComp : Component {

    //! Constructs a Mimic Component from some arbitrary callable object
    template <typename F>
    MimicComp(F&& f) : functor{std::forward<F>(f)}
    {
    }

    //! Delegates to functor
    void update(float dt);

    //! The functor!
    std::function<void(float)> functor;

} /*class MimicComp*/;
