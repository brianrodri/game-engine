#pragma once
#include "Component.h"
#include <aetee/aetee.h>

//! Implements a growable collection of _related_ components
class DynamicSetC : Component {
public:
    //! Default behavior
    DynamicSetC() = default;
    virtual ~DynamicSetC() = default;

    //! May be moved since this is just an array of pointers
    DynamicSetC(DynamicSetC&&) = default;
    DynamicSetC& operator=(DynamicSetC&&) = default;

    //! Can't be copied reliably
    DynamicSetC(const DynamicSetC&) = delete;
    DynamicSetC& operator=(const DynamicSetC&) = delete;

    //! Explicit constructor w/ pre-made Components
    template <typename... Ptr>
    DynamicSetC(Ptr&&... x) : members{std::move(x)...} {};

    //! Access a member directly through an index
    BaseComponent& operator[](size_t i);
    const BaseComponent& operator[](size_t i) const;

    template <typename CPtr> void attach(CPtr&& c)
    {
        members.emplace_back(std::move(c));
    }

    std::unique_ptr<BaseComponent> detach();

    /*
    //! Retrieve the underlying vector for more advaned functionality
    auto& as_vector();
    const auto& as_vector() const;
    */

    //! Calls update on each of the underlying components if they can be
    virtual void update(float dt) override;
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

private:
    std::vector<std::unique_ptr<BaseComponent>>  members;
} /*class DynamicSetC*/;
