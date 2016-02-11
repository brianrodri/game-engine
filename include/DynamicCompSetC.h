#pragma once
#include "Component.h"
#include <aetee/aetee.h>

//! Implements a growable collection of _related_ components
class DynamicCompSetC : Component {
public:
    //! Default behavior
    DynamicCompSetC() = default;
    virtual ~DynamicCompSetC() = default;
    DynamicCompSetC(DynamicCompSetC&&) = default;
    DynamicCompSetC& operator=(DynamicCompSetC&&) = default;

    //! Can't be copied due to possible complexities in the set
    DynamicCompSetC(const DynamicCompSetC&) = delete;
    DynamicCompSetC& operator=(const DynamicCompSetC&) = delete;

    //! Explicit constructor w/ pre-made Components
    template <typename Ptr...>
    DynamicCompSetC(Ptr&&... x) : members{std::move(x)...} {};

    //! Access a member directly through an index
    auto& operator[](size_t i);
    const auto& operator[](size_t i) const;

    //! Retrieve the underlying vector for more advaned functionality
    auto& as_vector();
    const auto& as_vector() const;

    //! Calls update on each of the underlying components if they can be
    virtual void update(float dt) override;
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

private:
    std::vector<std::unique_ptr<BaseComponent>>  members;
} /*class DynamicCompSetC*/;
