#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "util.hpp"

#include <imgui.h>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>

#define ADD_CFG_ITEM(type, name, def) \
 const uint32_t name = config::add_item<type>(util::cx_hash(#name), util::cx_hash(#type), def);
#define ADD_CFG_ITEM_VEC(type, datatype, size, name, def)                                                  \
 const uint32_t name = config::add_item<std::vector<datatype>>(util::cx_hash(#name), util::cx_hash(#type), \
															   util::create_filled_vector<datatype, size>(def));

namespace config {
	struct item {
		item() = default;
		item(const uint32_t name, const uint32_t type, const std::any var);

		~item() = default;

		template<typename T>
		void set(T value) {
			var.emplace<T>(value);
		}

		template<typename T>
		T& get() {
			return *reinterpret_cast<T*>(std::any_cast<T>(&var));
		}

		uint32_t name;
		uint32_t type;
		std::any var;
	};

	std::vector<item>& get_items();

	template<typename T>
	uint32_t add_item(const uint32_t name, const uint32_t type, const T def) {
		get_items().emplace_back(name, type, std::make_any<T>(def));
		return get_items().size() - 1u;
	}

	template<typename T>
	T& get(const uint32_t index) {
		return get_items()[index].get<T>();
	}

	item& get_item(const uint32_t index);
	int does_item_exist(const uint32_t name);

	bool list(std::vector<std::wstring>& configs);

	bool load(const std::wstring& name);
	bool save(const std::wstring& name);
	bool erase(const std::wstring& name);
}// namespace config

struct cfg_t {
	enum aimbot_target_selection_type : int {
		target_selection_fov,
		target_selection_distance,
		target_selection_health
	};

	enum aimbot_target_bone : int {
		target_head,
		target_thorax,
		target_body,
		target_randomize
	};

	enum box_type {
		box_2d,
		box_3d,
		box_italic
	};

	enum cham_material_type : int {
		material_default,
		material_flat,
		material_textured,
		material_glow,
		material_wireframe
	};

	// Aimbot
	ADD_CFG_ITEM(bool, aimbot, false)
	ADD_CFG_ITEM(int, aimbot_key, 0)
	ADD_CFG_ITEM(int, aimbot_fov, 90)
	ADD_CFG_ITEM(bool, aimbot_show_fov, false)
	ADD_CFG_ITEM(int, aimbot_max_distance, 0)
	ADD_CFG_ITEM(bool, aimbot_silent, true)
	ADD_CFG_ITEM(bool, aimbot_show_aim_point, false)
	ADD_CFG_ITEM(ImVec4, aimbot_aim_point_color, ImVec4(1.f, 1.f, 1.f, 1.f))
	ADD_CFG_ITEM(int, aimbot_target_selection, target_selection_fov)
	ADD_CFG_ITEM(int, aimbot_smoothing, 0)
	ADD_CFG_ITEM(bool, aimbot_only_visible, false)
	ADD_CFG_ITEM(bool, aimbot_prioritize_visible, true)
	ADD_CFG_ITEM(bool, aimbot_ignore_team, true)
	ADD_CFG_ITEM(int, aimbot_target_bone, target_head)
	ADD_CFG_ITEM(bool, aimbot_show_target_info, false)

	ADD_CFG_ITEM(bool, aim_no_recoil, false)
	ADD_CFG_ITEM(int, aim_recoil_control, 100)
	ADD_CFG_ITEM(bool, aim_no_spread, false)
	ADD_CFG_ITEM(bool, aim_no_sway, false)

	// Visuals
	ADD_CFG_ITEM(bool, visuals_radar, false)

	// Player visuals
	ADD_CFG_ITEM(bool, player_visuals, false)
	ADD_CFG_ITEM(ImVec4, player_visuals_color, ImVec4(0.87f, 0.15f, 0.15f, 1.0f))
	ADD_CFG_ITEM(ImVec4, player_hidden_visuals_color, ImVec4(0.72f, 0.12f, 0.12f, 1.0f))
	ADD_CFG_ITEM(ImVec4, player_scav_visuals_color, ImVec4(0.91f, 0.46f, 0.30f, 1.0f))
	ADD_CFG_ITEM(ImVec4, player_scav_hidden_visuals_color, ImVec4(0.74f, 0.37f, 0.23f, 1.0f))
	ADD_CFG_ITEM(ImVec4, scav_visuals_color, ImVec4(0.90f, 0.47f, 0.33f, 1.0f))
	ADD_CFG_ITEM(ImVec4, scav_hidden_visuals_color, ImVec4(0.75f, 0.38f, 0.27f, 1.0f))
	ADD_CFG_ITEM(ImVec4, boss_visuals_color, ImVec4(0.85f, 0.47f, 1.0f, 1.0f))
	ADD_CFG_ITEM(ImVec4, boss_hidden_visuals_color, ImVec4(0.65f, 0.36f, 0.76f, 1.0f))
	ADD_CFG_ITEM(ImVec4, friendly_visuals_color, ImVec4(0.25f, 0.92f, 0.15f, 1.0f))
	ADD_CFG_ITEM(ImVec4, friendly_hidden_visuals_color, ImVec4(0.2f, 0.75f, 0.12f, 1.0f))
	ADD_CFG_ITEM(int, player_visuals_box, box_italic)
	ADD_CFG_ITEM(bool, player_visuals_box_corner, false)
	ADD_CFG_ITEM(bool, player_visuals_name, false)
	ADD_CFG_ITEM(bool, player_visuals_health, false)
	ADD_CFG_ITEM(bool, player_visuals_ammo, false)
	ADD_CFG_ITEM(bool, player_visuals_held_item, false)
	ADD_CFG_ITEM(bool, player_visuals_distance, false)
	ADD_CFG_ITEM(bool, player_visuals_skeleton, false)
	ADD_CFG_ITEM(bool, player_visuals_value, false)
	ADD_CFG_ITEM(bool, player_visuals_items, false)
	ADD_CFG_ITEM(bool, player_visuals_state, false)
	ADD_CFG_ITEM(bool, player_streamer, false)
	ADD_CFG_ITEM(int, player_visuals_chams, material_default)
	ADD_CFG_ITEM(ImVec4, player_visuals_chams_color, ImVec4(1.0f, 0.0f, 1.0f, 0.3f))
	ADD_CFG_ITEM(ImVec4, player_visuals_chams_hidden_color, ImVec4(0.0f, 1.0f, 0.0f, 0.3f))

	// Item visuals

	// World visuals
};

extern cfg_t cfg;

#endif