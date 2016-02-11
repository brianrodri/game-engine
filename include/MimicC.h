#pragma once
#include "Component.h"
#include <functional>

//! Calls its input functor whenever updated
class MimicC : ProcessorComponent {
public:
    //! Constructs a Mimic Component from some arbitrary callable object
    template <typename F> MimicC(F&& f) : functor{std::forward<F>(f)}
    {
    }

    //! Virtual destruction
    virtual ~MimicC() = default;

    //! Processors must implement update
    virtual void update(float dt) override;

private:
    std::function<void(float)> functor;
} /*struct MimicC*/;
