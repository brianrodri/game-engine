#pragma once
#include <aetee/type_c.h>
#include <functional>

template
  < typename TComponentList
  , typename TTagList
  , typename TSignatureList
    >
struct Settings {

    // Useful aliases
    using ComponentList = TComponentList;
    using TagList = TTagList;
    using SignatureList = TSignatureList;

    // Component Queries
    static constexpr auto componentCount = aetee::length(ComponentList);
    template <typename T> static constexpr auto isComponent = aetee::type_exists_c<T, ComponentList>;
    template <typename T> static constexpr auto componentID = aetee::type_index_c<T, ComponentList>;

    // Tag Queries
    static constexpr auto tagCount = aetee::length(TagList);
    template <typename T> static constexpr auto isTag = aetee::type_exists_c<T, TagList>;
    template <typename T> static constexpr auto tagID = componentCount + aetee::type_index_c<T, TagList>;

    // Signature Queries
    static constexpr auto signatureCount = aetee::length(SignatureList);
    template <typename T> static constexpr auto isSignature = aetee::type_exists_c<T, SignatureList>;
    template <typename T> static constexpr auto signatureID = aetee::type_index_c<T, SignatureList>;

} /*struct Settings*/;
