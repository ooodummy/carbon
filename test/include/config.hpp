#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "carbon/util/hash.hpp"

#include <renderer/color.hpp>
#include <glm/glm.hpp>

#include <any>
#include <array>
#include <shared_mutex>
#include <vector>

namespace std::filesystem {
    class path;
}

constexpr inline auto MAX_TYPE_SIZE = 256;

#define ADD_CFG_ITEM(type, name, def)                                                               \
    static_assert(sizeof(type) < MAX_TYPE_SIZE);                                                    \
    const uint32_t name = settings::add_item<type>(carbon::cx_hash(#name), carbon::cx_hash(#type), def) \

#define VK_NONE 0xFF

#define ADD_CFG_ITEM_VEC(type, datatype, size, name, def) const uint32_t name = settings::add_item<std::vector<datatype>>(util::cx_hash(#name), util::cx_hash(#type), util::create_filled_vector<datatype, size>(def))

namespace settings {
    std::filesystem::path config_folder();
    inline std::shared_mutex config_mutex;

    struct item {
        item() = default;
        item(uint32_t name, uint32_t type);

        ~item() = default;

        template <typename T>
        void set(T value) {
            *reinterpret_cast<T*>(std::data(var)) = value;
        }

        template <typename T>
        T& get() {
            return *reinterpret_cast<T*>(std::data(var));
        }

        uint32_t name;
        uint32_t type;
        std::array<uint8_t, MAX_TYPE_SIZE> var;
    };

    std::vector<item>& get_items();

    template <typename T>
    uint32_t add_item(const uint32_t name, const uint32_t type, const T def) {
        std::unique_lock lock_guard(config_mutex);

        item new_cfg_item(name, type);
        std::memcpy(std::data(new_cfg_item.var), reinterpret_cast<const void*>(&def), sizeof(T));

        get_items().push_back(new_cfg_item);
        return get_items().size() - 1u;
    }

    template <typename T>
    T& get(const uint32_t index) {
        std::shared_lock lock_guard(config_mutex);

        return get_items()[index].get<T>();
    }

    item& get_item(uint32_t index);
    int does_item_exist(uint32_t name);

    bool list(std::vector<std::string>& configs);

    bool load(const std::string& name);
    bool save(const std::string& name);
    bool erase(const std::string& name);

    void update_item_filter_mask();
} // namespace settings

struct cham_config_t {
    bool enabled;
    int32_t material;
    glm::vec2 visible;
    glm::vec2 hidden;
};

struct config {
    // Aimbot
    ADD_CFG_ITEM(bool, aimbot, false);
    ADD_CFG_ITEM(bool, rage_mode, false);
    ADD_CFG_ITEM(float, aimbot_fov, 90);
    ADD_CFG_ITEM(bool, aimbot_show_fov, false);
    ADD_CFG_ITEM(glm::vec4, aimbot_fov_color, COLOR_WHITE);
    ADD_CFG_ITEM(float, aimbot_max_distance, 9999);
    ADD_CFG_ITEM(bool, aimbot_silent, false);
    ADD_CFG_ITEM(bool, aimbot_draw_target_line, false);
    ADD_CFG_ITEM(bool, aimbot_show_bullet_drop, false);

    //ADD_CFG_ITEM(int32_t, aimbot_target_selection, target_selection_fov);
    ADD_CFG_ITEM(float, aimbot_smoothing, 0.0f);
    ADD_CFG_ITEM(bool, aimbot_only_visible, false);
    //ADD_CFG_ITEM(bool, aimbot_prioritize_visible, false);
    ADD_CFG_ITEM(bool, aimbot_randomize_point, false);
    ADD_CFG_ITEM(bool, aimbot_randomize_target_bone, false);
    ADD_CFG_ITEM(bool, aimbot_ignore_team, false);
    //ADD_CFG_ITEM(int32_t, aimbot_target_bone, target_head);

    // Visuals
    ADD_CFG_ITEM(bool, hide_from_capture, false);

    // Player visuals
    ADD_CFG_ITEM(bool, player_visuals, false);

    ADD_CFG_ITEM(glm::vec4, player_visuals_color, COLOR_WHITE);
    ADD_CFG_ITEM(glm::vec4, player_hidden_visuals_color, COLOR_RED);

    ADD_CFG_ITEM(glm::vec4, disconnected_player_color, COLOR_AZURE);

    ADD_CFG_ITEM(glm::vec4, player_scav_visuals_color, COLOR_ORANGE);
    ADD_CFG_ITEM(glm::vec4, player_scav_hidden_visuals_color, COLOR_WHITE);

    ADD_CFG_ITEM(glm::vec4, scav_visuals_color, COLOR_YELLOW);
    ADD_CFG_ITEM(glm::vec4, scav_hidden_visuals_color, COLOR_WHITE);

    ADD_CFG_ITEM(glm::vec4, boss_visuals_color, COLOR_RED);
    ADD_CFG_ITEM(glm::vec4, boss_hidden_visuals_color, COLOR_YELLOW);

    ADD_CFG_ITEM(glm::vec4, friendly_visuals_color, COLOR_GREEN);
    ADD_CFG_ITEM(glm::vec4, friendly_hidden_visuals_color, COLOR_GREEN);

    ADD_CFG_ITEM(glm::vec4, player_streamer_flag_color, COLOR_RED);
    ADD_CFG_ITEM(glm::vec4, player_group_hash_flag_color, COLOR_LIME);

    //ADD_CFG_ITEM(int32_t, player_visuals_box_type, box_2d);

    ADD_CFG_ITEM(float, player_visuals_box_rounding, 0.0f);
    ADD_CFG_ITEM(bool, player_visuals_chams, false);
    ADD_CFG_ITEM(bool, player_visuals_name, false);
    ADD_CFG_ITEM(bool, player_visuals_scav_name, false);
    ADD_CFG_ITEM(bool, player_visuals_health, false);
    ADD_CFG_ITEM(bool, player_visuals_ammo, false);
    ADD_CFG_ITEM(bool, player_visuals_held_item, false);
    ADD_CFG_ITEM(bool, player_visuals_account_type, false);
    ADD_CFG_ITEM(bool, player_visuals_group_hash, false);
    // ADD_CFG_ITEM(bool, player_visuals_disconnected, false);
    // ADD_CFG_ITEM(bool, player_visuals_streamer, false);
    ADD_CFG_ITEM(bool, player_visuals_distance, false);
    ADD_CFG_ITEM(bool, player_visuals_skeleton, false);
    ADD_CFG_ITEM(bool, player_visuals_direction, false);

    ADD_CFG_ITEM(float, player_visuals_box_max_distance, 9999);
    ADD_CFG_ITEM(float, player_visuals_skeleton_max_distance, 9999);
    ADD_CFG_ITEM(float, player_visuals_direction_max_distance, 9999);

    ADD_CFG_ITEM(bool, player_visuals_oof, false);
    ADD_CFG_ITEM(int32_t, player_visuals_oof_size, 10);
    ADD_CFG_ITEM(int32_t, player_visuals_oof_distance, 50);

    ADD_CFG_ITEM(bool, player_visuals_target_info, false);
    ADD_CFG_ITEM(bool, player_visuals_target_info_stats, false);
    // ADD_CFG_ITEM(bool, player_visuals_target_info_value, false);
    // ADD_CFG_ITEM(bool, player_visuals_target_info_gear, false);
    // ADD_CFG_ITEM(bool, player_visuals_target_info_held_item, false);
    // ADD_CFG_ITEM(bool, player_visuals_target_info_items, false);
    // ADD_CFG_ITEM(bool, player_visuals_target_info_effects, false);

    //ADD_CFG_ITEM(int32_t, player_visuals_chams_type, material_wireframe);

    // Loot visuals
    ADD_CFG_ITEM(bool, loot_containers, false);
    ADD_CFG_ITEM(bool, loot_items, false);
    ADD_CFG_ITEM(bool, corpses, false);

    ADD_CFG_ITEM(bool, flea_price, false);
    ADD_CFG_ITEM(bool, trader_price, false);
    ADD_CFG_ITEM(bool, weapon_mods_trader_price, false);
    ADD_CFG_ITEM(bool, user_filter_color_override, false);

    ADD_CFG_ITEM(bool, loot_containers_draw_name, false);
    ADD_CFG_ITEM(bool, loot_fov_decrease_alpha, false);
    ADD_CFG_ITEM(bool, loot_fov_hide_list, false);
    ADD_CFG_ITEM(bool, loot_show_fov, false);
    ADD_CFG_ITEM(float, loot_detail_fov, 35.0f);
    ADD_CFG_ITEM(glm::vec4, loot_fov_color, COLOR_ORANGE);

    ADD_CFG_ITEM(float, item_bounds_distance, 50.0f);
    ADD_CFG_ITEM(float, container_bounds_distance, 50.0f);

    ADD_CFG_ITEM(glm::vec4, container_bounds_color, COLOR_WHITE);

    ADD_CFG_ITEM(glm::vec4, pmc_corpses_color, COLOR_WHITE);
    ADD_CFG_ITEM(glm::vec4, scav_corpses_color, COLOR_WHITE);

    ADD_CFG_ITEM(glm::vec4, loot_rarity_uncommon_color, COLOR_GREY);
    ADD_CFG_ITEM(glm::vec4, loot_rarity_rare_color, COLOR_AZURE);
    ADD_CFG_ITEM(glm::vec4, loot_rarity_epic_color, COLOR_CYAN);
    ADD_CFG_ITEM(glm::vec4, loot_rarity_user_filter_color, COLOR_LIME);
    ADD_CFG_ITEM(glm::vec4, loot_rarity_legendary_color, COLOR_RED);
    ADD_CFG_ITEM(glm::vec4, loot_rarity_quest_color, COLOR_GREEN);

    ADD_CFG_ITEM(int32_t, loot_rarity_rare_price, 65000);
    ADD_CFG_ITEM(int32_t, loot_rarity_epic_price, 75000);
    ADD_CFG_ITEM(int32_t, loot_rarity_legendary_price, 110000);

    ADD_CFG_ITEM(int32_t, loot_minimum_price, 0);

    ADD_CFG_ITEM(bool, loot_item_filter_price, false);

    ADD_CFG_ITEM(bool, loot_item_filter_gun, false);
    ADD_CFG_ITEM(bool, loot_item_filter_wearable, false);
    ADD_CFG_ITEM(bool, loot_item_filter_ammo_box, false);
    ADD_CFG_ITEM(bool, loot_item_filter_keys, false);
    ADD_CFG_ITEM(bool, loot_item_filter_ammo, false);
    ADD_CFG_ITEM(bool, loot_item_filter_grenade, false);
    ADD_CFG_ITEM(bool, loot_item_filter_mods, false);
    ADD_CFG_ITEM(bool, loot_item_filter_provisions, false);
    ADD_CFG_ITEM(bool, loot_item_filter_no_flea, false);
    ADD_CFG_ITEM(bool, loot_item_filter_armor, false);
    ADD_CFG_ITEM(bool, loot_item_filter_rig, false);
    ADD_CFG_ITEM(bool, loot_item_filter_backpack, false);
    ADD_CFG_ITEM(bool, loot_item_filter_meds, false);
    ADD_CFG_ITEM(bool, loot_item_filter_injectors, false);
    ADD_CFG_ITEM(bool, loot_item_filter_barter, false);
    ADD_CFG_ITEM(bool, loot_item_filter_suppressor, false);
    ADD_CFG_ITEM(bool, loot_item_filter_helmet, false);
    ADD_CFG_ITEM(bool, loot_item_filter_money, false);

    //using filter_ids_type = std::list<std::pair<std::wstring, std::string>>;
    //ADD_CFG_ITEM(filter_ids_type, loot_item_filter_ids, {});

    // Quest visuals
    ADD_CFG_ITEM(bool, quests_show_locations, false);
    ADD_CFG_ITEM(bool, quests_show_quest_items, false);
    ADD_CFG_ITEM(bool, quests_show_inactive_quest_items, false);
    ADD_CFG_ITEM(bool, pickup_inactive_quest_items, false);
    ADD_CFG_ITEM(glm::vec4, quests_locations_color, COLOR_WHITE);

    // World visuals
    ADD_CFG_ITEM(bool, world_danger_zones, false);
    ADD_CFG_ITEM(bool, world_claymores, false);
    ADD_CFG_ITEM(bool, world_exfils, false);
    ADD_CFG_ITEM(bool, world_show_unuseable_exfils, false);
    ADD_CFG_ITEM(bool, world_interactables, false);
    ADD_CFG_ITEM(bool, world_grenades, false);
    ADD_CFG_ITEM(int32_t, prediction_simulation_iterations, 10);
    ADD_CFG_ITEM(float, prediction_simulation_fixed_delta_time, 0.02f);

    ADD_CFG_ITEM(float, world_danger_zones_distance, 50.0f);
    ADD_CFG_ITEM(float, world_claymores_distance, 40.0f);
    ADD_CFG_ITEM(float, world_interactables_distance, 500.0f);
    ADD_CFG_ITEM(float, world_interactables_bounds_distance, 25.0f);

    ADD_CFG_ITEM(glm::vec4, world_danger_zones_color, COLOR_RED);
    ADD_CFG_ITEM(glm::vec4, world_grenade_color, COLOR_WHITE);

    ADD_CFG_ITEM(glm::vec4, world_exfil_not_present_color, COLOR_WHITE);
    ADD_CFG_ITEM(glm::vec4, world_exfil_uncomplete_requirements_manual_color, COLOR_GREY);
    ADD_CFG_ITEM(glm::vec4, world_exfil_uncomplete_requirements_shared_timer_color, COLOR_ORANGE);
    ADD_CFG_ITEM(glm::vec4, world_exfil_uncomplete_requirements_coop_color, COLOR_AZURE);
    ADD_CFG_ITEM(glm::vec4, world_exfil_countdown_color, COLOR_YELLOW);
    ADD_CFG_ITEM(glm::vec4, world_exfil_regular_mode_color, COLOR_GREEN);
    ADD_CFG_ITEM(glm::vec4, world_exfil_pending_color, COLOR_YELLOW);
    ADD_CFG_ITEM(glm::vec4, world_exfil_awaits_manual_activation_color, COLOR_GREY);

    // Radar visuals
    ADD_CFG_ITEM(bool, radar, false);
    ADD_CFG_ITEM(float, radar_scale, 0.25f);
    ADD_CFG_ITEM(float, radar_radius, 100.0f);
    //ADD_CFG_ITEM(float, radar_ring_spacing, 20.0f);
    ADD_CFG_ITEM(float, radar_view_line_length, 50.0f);
    //ADD_CFG_ITEM(float, radar_entity_size, 5.0f);
    //ADD_CFG_ITEM(float, radar_elevation_size, 3.0f);
    ADD_CFG_ITEM(bool, radar_draw_fov, false);
    ADD_CFG_ITEM(bool, radar_draw_rings, false);
    ADD_CFG_ITEM(bool, radar_draw_map_north, false);
    ADD_CFG_ITEM(bool, radar_draw_elevation, false);

    // Local chams
    ADD_CFG_ITEM(bool, local_chams, false);
    //ADD_CFG_ITEM(int32_t, local_chams_type, material_wireframe);
    ADD_CFG_ITEM(glm::vec4, visuals_local_chams_visible_color, COLOR_CYAN);
    ADD_CFG_ITEM(glm::vec4, visuals_local_chams_hidden_color, COLOR_GREEN);

    // Local held weapon chams
    ADD_CFG_ITEM(bool, local_weapon_chams, false);
    //ADD_CFG_ITEM(int32_t, local_weapon_chams_type, material_wireframe);
    ADD_CFG_ITEM(glm::vec4, visuals_local_weapon_chams_visible_color, COLOR_CYAN);
    ADD_CFG_ITEM(glm::vec4, visuals_local_weapon_chams_hidden_color, COLOR_GREEN);

    // Local ammo chams
    ADD_CFG_ITEM(bool, local_ammo_chams, false);
    //ADD_CFG_ITEM(int32_t, local_ammo_chams_type, material_wireframe);
    ADD_CFG_ITEM(glm::vec4, visuals_local_ammo_chams_visible_color, COLOR_CYAN);
    ADD_CFG_ITEM(glm::vec4, visuals_local_ammo_chams_hidden_color, COLOR_GREEN);

    // Misc visuals
    ADD_CFG_ITEM(bool, visuals_crosshair, false);
    ADD_CFG_ITEM(int32_t, visuals_crosshair_size, 5);
    ADD_CFG_ITEM(int32_t, visuals_crosshair_gap, 0);
    ADD_CFG_ITEM(int32_t, visuals_crosshair_thickness, 1);
    ADD_CFG_ITEM(bool, visuals_ammo_hud, false);
    ADD_CFG_ITEM(bool, visuals_bullet_tracers, false);
    //ADD_CFG_ITEM(int32_t, visuals_bullet_tracer_type, tracers_render_line);
    ADD_CFG_ITEM(float, visuals_bullet_tracer_duration, 3.0f);
    ADD_CFG_ITEM(float, visuals_bullet_tracer_size, 0.01f);
    ADD_CFG_ITEM(bool, visuals_barrel_line, false);
    ADD_CFG_ITEM(bool, visuals_debug_info, false);
    ADD_CFG_ITEM(bool, visuals_feature_info, false);
    ADD_CFG_ITEM(bool, visuals_show_bearing, false);

    ADD_CFG_ITEM(glm::vec4, visuals_crosshair_color, COLOR_WHITE);
    ADD_CFG_ITEM(glm::vec4, visuals_barrel_line_color, COLOR_RED);
    ADD_CFG_ITEM(glm::vec4, visuals_bullet_tracer_visible_color, COLOR_WHITE);
    ADD_CFG_ITEM(glm::vec4, visuals_bullet_tracer_hidden_color, COLOR_WHITE);

    // Misc
    ADD_CFG_ITEM(bool, skills, false);

    ADD_CFG_ITEM(bool, strength_sprint_speed, false);
    ADD_CFG_ITEM(float, jump_height_multiplier, 0.0f);

    ADD_CFG_ITEM(bool, chamber_always_checked, false);
    ADD_CFG_ITEM(bool, weapon_mastery_override, false);
    ADD_CFG_ITEM(int32_t, weapon_mastery_level, 0);
    // ADD_CFG_ITEM(float, mag_unload_time_factor, 0.01f);
    // ADD_CFG_ITEM(float, mag_check_time_factor, 0.01f);
    // ADD_CFG_ITEM(float, mag_load_time_factor, 0.01f);
    ADD_CFG_ITEM(bool, instant_offline_deploy, false);

    ADD_CFG_ITEM(bool, no_inertia, false);

    ADD_CFG_ITEM(float, item_pick_up_delay_factor, 0.001f);
    ADD_CFG_ITEM(float, loot_raycast_distance, 3.5f);
    ADD_CFG_ITEM(float, door_raycast_distance, 2.5f);
    ADD_CFG_ITEM(float, grenade_force_multiplier, 1.25f);
    ADD_CFG_ITEM(bool, free_look, false);
    ADD_CFG_ITEM(bool, med_effect_using_panel, false);
    ADD_CFG_ITEM(bool, loot_through_walls, false);

    ADD_CFG_ITEM(bool, no_sway, false);
    // ADD_CFG_ITEM(bool, no_spread, false);
    // ADD_CFG_ITEM(bool, no_recoil, false);
    ADD_CFG_ITEM(bool, instant_ads, false);

    ADD_CFG_ITEM(bool, always_painkiller, false);
    ADD_CFG_ITEM(bool, instant_plant, false);
    ADD_CFG_ITEM(bool, increase_stamina_restore, false);
    ADD_CFG_ITEM(bool, fatigue_bypass, false);
    ADD_CFG_ITEM(bool, infinite_stamina, false);
    ADD_CFG_ITEM(bool, no_weight_limit, false);
    ADD_CFG_ITEM(float, stamina_restore_rate_multiplier, 75.0f);

    ADD_CFG_ITEM(bool, no_shot_spread, false);
    ADD_CFG_ITEM(bool, short_barrel, false);
    // ADD_CFG_ITEM(bool, no_malfunctions, false);
    ADD_CFG_ITEM(float, weapon_recoil_strength, 1.0f);
    ADD_CFG_ITEM(float, weapon_swap_speed, 3.0f);
    ADD_CFG_ITEM(float, weapon_aim_movment_speed, 3.0f);
    ADD_CFG_ITEM(float, weapon_malfunction_fix_speed, 2.0f);
    ADD_CFG_ITEM(float, weapon_reload_speed, 2.0f);
    ADD_CFG_ITEM(bool, weapon_stiff_draw, false);

    ADD_CFG_ITEM(bool, stationary_weapon_no_limit, false);

    ADD_CFG_ITEM(float, fov_zoom_factor, 2.0f);

    ADD_CFG_ITEM(bool, night_vision, false);
    ADD_CFG_ITEM(glm::vec4, night_vision_color, COLOR_CYAN);

    ADD_CFG_ITEM(bool, thermal_vision, false);

    ADD_CFG_ITEM(bool, custom_weather, false);
    ADD_CFG_ITEM(bool, custom_time, false);
    ADD_CFG_ITEM(float, current_time, 12.0f);
    ADD_CFG_ITEM(float, fog_level, 0.0f);
    ADD_CFG_ITEM(float, rain_level, 0.0f);
    ADD_CFG_ITEM(float, cloud_density, -1);
    ADD_CFG_ITEM(float, lightning_thunder_probability, 0.0f);

    // TOD_Sky
    ADD_CFG_ITEM(bool, tod_sky_manipulation, false);
    ADD_CFG_ITEM(glm::vec4, tod_sky_sun_sky_color, COLOR_WHITE);
    ADD_CFG_ITEM(glm::vec4, tod_sky_moon_sky_color, COLOR_WHITE);
    ADD_CFG_ITEM(glm::vec4, tod_sky_cloud_color, COLOR_WHITE);

    // Hooks
    ADD_CFG_ITEM(bool, hide_session_id, true);
    ADD_CFG_ITEM(bool, hit_markers, false);
    //ADD_CFG_ITEM(int32_t, hit_marker_indicator, hit_indicator_3d);
    ADD_CFG_ITEM(glm::vec4, hit_marker_color, COLOR_WHITE);

    // 2d, 3d hitmarker settings
    ADD_CFG_ITEM(int32_t, hit_marker_size, 6);
    ADD_CFG_ITEM(int32_t, hit_marker_gap, 0);
    ADD_CFG_ITEM(int32_t, hit_marker_thickness, 1);

    // 3d normal aligned hitmarker settings
    ADD_CFG_ITEM(float, hit_marker_3d_size, 0.1f);
    ADD_CFG_ITEM(float, hit_marker_3d_gap, 0.01f);

    ADD_CFG_ITEM(float, hit_marker_duration, 3.0f);
    //ADD_CFG_ITEM(int32_t, hit_marker_sound, hit_marker_sound::hit_marker_cod);
    ADD_CFG_ITEM(float, hit_marker_volume, 0.3f);
    ADD_CFG_ITEM(bool, no_visor, false);
    ADD_CFG_ITEM(bool, no_shot_animations, false);
    // ADD_CFG_ITEM(bool, god_mode_resolver, false);
    // ADD_CFG_ITEM(int32_t, god_mode_resolver_type, player_stance::prone);
    ADD_CFG_ITEM(bool, disassemble_guns_in_raid, false);
    ADD_CFG_ITEM(bool, use_broken_guns_in_raid, false);
    ADD_CFG_ITEM(bool, bind_item_anywhere, false);
    ADD_CFG_ITEM(bool, mags_always_checked, false);
    ADD_CFG_ITEM(bool, mags_show_count, false);
    ADD_CFG_ITEM(bool, shoot_while_running, false);
    ADD_CFG_ITEM(bool, no_aim_punch, false);
    ADD_CFG_ITEM(bool, no_screen_effects, false);
    ADD_CFG_ITEM(bool, no_inventory_blur, false);
    ADD_CFG_ITEM(bool, no_stun, false);
    ADD_CFG_ITEM(bool, multi_search, false);
    ADD_CFG_ITEM(bool, always_searched, false);
    ADD_CFG_ITEM(bool, can_always_lean, false);
    ADD_CFG_ITEM(bool, far_lean, false);
    ADD_CFG_ITEM(bool, no_character_slowdowns, false);
    ADD_CFG_ITEM(bool, instant_pose_change, false);
    ADD_CFG_ITEM(bool, no_armor_penalties, false);
    ADD_CFG_ITEM(bool, no_obstacle_collision, false);

    // Exploits
    ADD_CFG_ITEM(bool, steal_containers, false);
    ADD_CFG_ITEM(bool, speed_hacks, false);
    //ADD_CFG_ITEM(int32_t, speed_hack_type, speed_hack_direct);
    ADD_CFG_ITEM(bool, speed_hack_spoof_dt, false);
    ADD_CFG_ITEM(bool, speed_hack_force, false);
    ADD_CFG_ITEM(float, speed_hack_multiplier, 1.5f);

    ADD_CFG_ITEM(bool, no_afk_timer, false);
    ADD_CFG_ITEM(bool, kd_dropper, false);
    ADD_CFG_ITEM(int32_t, kd_dropper_map, 0);
    ADD_CFG_ITEM(bool, always_grounded, false);
    ADD_CFG_ITEM(bool, no_falling, false);
    ADD_CFG_ITEM(bool, removable_dogtag, false);
    ADD_CFG_ITEM(bool, show_kills_no_tag, false);
    ADD_CFG_ITEM(bool, streamer_mode, true);
    ADD_CFG_ITEM(bool, streamer_mode_all, false);
    ADD_CFG_ITEM(bool, god_mode, false);
    ADD_CFG_ITEM(bool, no_tinnitus, false);
    ADD_CFG_ITEM(bool, no_breathing, false);

    // Keybinds
    ADD_CFG_ITEM(int32_t, aimbot_key, VK_NONE);
    ADD_CFG_ITEM(int32_t, aimbot_free_aim_key, VK_NONE);

    ADD_CFG_ITEM(int32_t, loot_show_contents_key, VK_NONE);

    ADD_CFG_ITEM(int32_t, night_vision_key, VK_NONE);
    ADD_CFG_ITEM(int32_t, thermal_vision_key, VK_NONE);

    ADD_CFG_ITEM(int32_t, fov_zoom_out_key, VK_NONE);
    ADD_CFG_ITEM(int32_t, fov_zoom_in_key, VK_NONE);

    ADD_CFG_ITEM(int32_t, target_bone_key, VK_NONE);
    ADD_CFG_ITEM(int32_t, resolver_key, VK_NONE);
    //ADD_CFG_ITEM(int32_t, resolver_type_key, ImGuiKey_None);
    ADD_CFG_ITEM(int32_t, rage_mode_key, VK_NONE);
    ADD_CFG_ITEM(int32_t, random_bone_key, VK_NONE);
    ADD_CFG_ITEM(int32_t, god_mode_key, VK_NONE);
    ADD_CFG_ITEM(int32_t, streamer_mode_key, VK_NONE);

    ADD_CFG_ITEM(int32_t, kd_dropper_key, VK_NONE);

    ADD_CFG_ITEM(int32_t, no_falling_key, VK_NONE);
    ADD_CFG_ITEM(int32_t, always_grounded_key, VK_NONE);

    ADD_CFG_ITEM(int32_t, loot_teleport_key, VK_NONE);
    ADD_CFG_ITEM(int32_t, speed_hack_key, VK_NONE);
    ADD_CFG_ITEM(int32_t, click_teleport_key, VK_NONE);

    // Feature Flags
    ADD_CFG_ITEM(bool, outline_text, false);
};

extern config cfg;

#endif