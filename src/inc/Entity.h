#ifndef HEADER_GAME_ENGINE_SRC_INC_ENTITY_H_INCLUDED
#define HEADER_GAME_ENGINE_SRC_INC_ENTITY_H_INCLUDED
#include <experimental/type_traits>
#include "Component.h"
#include "FactoryTuple.h"
#include <aetee/aetee.h>

template <typename... C>
class Entity : public FactoryTuple<C...> {
    static_assert((std::experimental::is_base_of_v<BaseComponent, C> && ...));

    struct isRenderableFunctor {
        template <size_t I>
        auto operator()(aetee::index_constant_t<I>) const
        {
            using K = std::tuple_element_t<I, std::tuple<C...>>;
            return std::experimental::disjunction<
                    std::is_base_of<PainterComponent, K>
                  , std::is_base_of<Component, K>
                    >{};
        }
    };

    struct isUpdatableFunctor {
        template <size_t I>
        auto operator()(aetee::index_constant_t<I>) const
        {
            using K = std::tuple_element_t<I, std::tuple<C...>>;
            return std::experimental::disjunction<
                    std::is_base_of<ProcessorComponent, K>
                  , std::is_base_of<Component, K>
                    >{};
        }
    };

public:
    template <typename... F>
    constexpr Entity(F&&... f) : FactoryTuple<C...>{std::forward<F>(f)...}
    {
    }

    virtual ~Entity()
    {
    }

    void update(float dt = {})
    {
        aetee::for_each(
            aetee::filter(
                aetee::index_sequence_c<sizeof...(C)>
              , isUpdatableFunctor{}
                )
          , [=](auto i) { (*this)[i].update(dt); }
            );
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        aetee::for_each(
            aetee::filter(
                aetee::index_sequence_c<sizeof...(C)>
              , isRenderableFunctor{}
                )
          , [&](auto i) { (*this)[i].draw(target, {states}); }
            );
    }
};

#endif
