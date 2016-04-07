#pragma once
#include <functional>

//! Calls its input function whenever updated
struct CopyistComp {

    //! Constructs a Mimic Component from some arbitrary callable object
    template<typename F>
    CopyistComp(F&& f) : functor{std::forward<F>(f)}
    {
    }

    //! Delegates to functor
    void draw(sf::RenderTarget&, sf::RenderStates) const;

    //! The functor!
    std::function<void(sf::RenderTarget&, sf::RenderStates)> functor;

} /*class CopyistComp*/;

