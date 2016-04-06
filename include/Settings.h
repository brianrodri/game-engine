#pragma once
#include <aetee/aetee.h>
#include <type_traits>
#include <boost/variant.hpp>
#include "SettingsConcrete.h"

namespace aet = aetee;

namespace settings {

// Helper-types
using ComponentVariant = typename decltype(aet::rebind<boost::variant>.direct(ComponentList{}))::type;
using TagVariant = typename decltype(aet::rebind<boost::variant>.direct(TagList{}))::type;
using SignatureVariant = typename decltype(aet::rebind<boost::variant>.direct(SignatureList{}))::type;

using ComponentTypeVariant = typename decltype(aet::rebind<boost::variant>(aet::type_c<ComponentList>))::type;
using TagTypeVariant = typename decltype(aet::rebind<boost::variant>(aet::type_c<TagList>))::type;
using SignatureTypeVariant = typename decltype(aet::rebind<boost::variant>(aet::type_c<SignatureList>))::type;

// Component Queries
static constexpr auto componentCount = aet::len_c<ComponentList>;
template <typename T> static constexpr auto isComponent = aet::type_exists_c<T, ComponentList>;
template <typename T> static constexpr auto componentID = aet::type_idx_c<T, ComponentList>;

// Tag Queries
static constexpr auto tagCount = aet::len_c<TagList>;
template <typename T> static constexpr auto isTag = aet::type_exists_c<T, TagList>;
template <typename T> static constexpr auto tagID = componentCount + aet::type_idx_c<T, TagList>;

// Signature Queries
static constexpr auto signatureCount = aet::len_c<SignatureList>;
template <typename T> static constexpr auto isSignature = aet::type_exists_c<T, SignatureList>;
template <typename T> static constexpr auto signatureID = componentCount + tagCount + aet::type_idx_c<T, SignatureList>;

} /*namespace settings*/;
