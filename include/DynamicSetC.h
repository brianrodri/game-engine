#pragma once
#include "Component.h"
#include <bitset>
#include <aetee/aetee.h>

//! Implements a growable collection of _related_ components
class DynamicSetC : Component {
    static constexpr const std::bitset<2> updatable{1};
    static constexpr const std::bitset<2> renderable{2};

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

    template <typename C> void attach(std::unique_ptr<C> c)
    {
        members.emplace_back(std::move(c));
        member_functionality.push_back(0);
        if (std::is_base_of<ProcessorComponent, C>::value) {
            member_functionality.back() |= updatable;
        }
        if (std::is_base_of<PainterComponent, C>::value) {
            member_functionality.back() |= renderable;
        }
    }

    template <typename CPtr> void attach(CPtr&& c)
    {
        using C = std::remove_pointer_t<std::decay_t<CPtr>>;
        members.emplace_back(std::move(c));
        member_functionality.push_back(0);
        if (std::is_base_of<ProcessorComponent, C>::value) {
            member_functionality.back() |= updatable;
        }
        if (std::is_base_of<PainterComponent, C>::value) {
            member_functionality.back() |= renderable;
        }
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
    std::vector<std::bitset<2>>  member_functionality;
} /*class DynamicSetC*/;
