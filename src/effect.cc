#include "effect.hh"

#include <stdexcept> 

#include <obs/obs-module.h>

struct effect_data {
    obs_source_t* source_handler;
    gs_effect_t* effect_handler;
};

static void* censor_create(obs_data_t* settings, obs_source_t* context) {
    effect_data* user_data = new effect_data;
    user_data->source_handler = context;

    // Create the graphics context 
    obs_enter_graphics();
    const char* effect_path = obs_module_file("test.effect");
    user_data->effect_handler = gs_effect_create_from_file(effect_path, nullptr);
    if(user_data->effect_handler == nullptr) {
        throw std::runtime_error("gs_effect_create_from_file returned nullptr");
    }
    obs_leave_graphics();

    censor_update(user_data, settings);

    return user_data;
}

static void censor_destroy(void* data) {
    auto* user_data = static_cast<effect_data*>(data);
    
    // Destroy the effect and clean the user data
    if(user_data->effect_handler != nullptr) {
        obs_enter_graphics();
        gs_effect_destroy(user_data->effect_handler);
        obs_leave_graphics();
    }
    delete user_data;
}

static void censor_render(void *data, gs_effect_t* effect) {
    auto* user_data = static_cast<effect_data*>(data);
}

static void censor_update(void *data, obs_data_t* settings) { 

}

static obs_properties_t* censor_properties(void *data) {
    auto* properties = obs_properties_create();
    return properties;
}

static void censor_defaults(obs_data_t *settings) {

}

static const char* censor_get_name(void* data) {
    return "Ditector";
}

constexpr obs_source_info censor_effect = {
	.id = "ditector_censor_effect",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CAP_OBSOLETE,
	.get_name = censor_get_name,
	.create = censor_create,
	.destroy = censor_destroy,
	.video_render = censor_render,
	.update = censor_update,
	.get_properties = censor_properties,
	.get_defaults = censor_defaults,
};

void register_effect() {
    obs_register_source(&censor_effect);
};