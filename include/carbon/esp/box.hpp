#ifndef CARBON_ESP_BOX_HPP
#define CARBON_ESP_BOX_HPP

#include <vector>
#include <memory>

#include <glm/glm.hpp>

// TODO: I feel this currently layout is idiotic how should I configure a fancy customizable ESP system?
namespace esp {
	enum bounding_box_type {
		box_2d,
		box_3d,
		box_2d_dynamic
	};

	struct bounds {
		glm::vec3 center;
		glm::vec3 extents;
	};

	// Indicator that can be attached to a bounding box
	class box_indicator {
		virtual void draw();
	};

	class player_name_indicator {
		virtual void draw();
	};

	class player_health_indicator {
		virtual void draw();
	};

	class player_ammo_indicator {
		virtual void draw();
	};

	class player_held_item_indicator {
		virtual void draw();
	};

	class bounding_box_renderer {
	public:

	protected:
		bounding_box_type type_;

		std::vector<std::unique_ptr<box_indicator*>> top_;
		std::vector<std::unique_ptr<box_indicator*>> bottom_;
		std::vector<std::unique_ptr<box_indicator*>> left_;
		std::vector<std::unique_ptr<box_indicator*>> right_;
	};

	class player_bounding_box_renderer : public bounding_box_renderer {
	public:
		player_bounding_box_renderer() {
			top_.emplace_back(std::make_unique<player_name_indicator>());
			right_.emplace_back(std::make_unique<player_health_indicator>());
			bottom_.emplace_back(std::make_unique<player_ammo_indicator>());
			bottom_.emplace_back(std::make_unique<player_held_item_indicator>());

		}
	};
}

#endif