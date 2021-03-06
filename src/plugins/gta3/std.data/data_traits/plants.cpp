/*
 * Copyright (C) 2015  LINK/2012 <dma_2012@hotmail.com>
 * Licensed under the MIT License, see LICENSE at top level directory.
 * 
 */
#include <stdinc.hpp>
#include "../data_traits.hpp"
using namespace modloader;

struct plants_traits : public data_traits
{
    static const bool has_sections      = false;
    static const bool per_line_section  = false;

    struct dtraits : modloader::dtraits::OpenFile
    {
        static const char* what() { return "plants surface properties"; }
    };
    
    using detour_type = modloader::OpenFileDetour<0x5DD3D1, dtraits>;

    using key_type      = std::pair<std::size_t, int>;
    using value_type    = data_slice<std::string,
                            int, int, int, int, rgb, pack<color, 3>,
                            vec3, vec3, real_t>;

    key_type key_from_value(const value_type& value)
    {
        return key_type(modloader::hash(get<0>(value)), get<1>(value)); 
    }
};

using plants_store = gta3::data_store<plants_traits, std::map<
                        plants_traits::key_type, plants_traits::value_type
                        >>;

static auto xinit = initializer([](DataPlugin* plugin_ptr)
{
    if(!gvm.IsSA())
        return;

    auto ReloadPlantsDat = [plugin_ptr]
    {
        if(!injector::cstd<char()>::call<0x5DD780>()) // CPlantMgr::ReloadConfig
            plugin_ptr->Log("Failed to refresh plant manager");
    };

    plugin_ptr->AddMerger<plants_store>("plants.dat", true, false, false, reinstall_since_load, gdir_refresh(ReloadPlantsDat));
});

