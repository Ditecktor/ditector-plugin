#include "effect.hh"

#include <stdexcept> 

#include <obs/obs-module.h>

static void* censor_create(obs_data_t* settings, obs_source_t* context);
static void censor_destroy(void* data);
static void censor_render(void *data, gs_effect_t* effect);
static void censor_update(void *data, obs_data_t* settings);
static obs_properties_t* censor_properties(void *data);
static void censor_defaults(obs_data_t *settings);
static const char* censor_get_name(void* data);

constexpr char* ACTIVE_PROPERTY = "Active";

struct effect_data {
    obs_source_t* source_handler;
    gs_effect_t* effect_handler;

    bool is_active = false;

    bool is_all_ok = true;
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
    /// TODO: think about using passed effect instead of the one stored on user data

    auto* user_data = static_cast<effect_data*>(data);

    // If the filter is not active, do nothing
    if(!user_data->is_active) {
        return;
    }

    // Start rendering
    bool ok = obs_source_process_filter_begin(user_data->source_handler, 
                                              GS_RGBA, 
                                              OBS_ALLOW_DIRECT_RENDERING);
    if(!ok) {
        /// TODO: error popup or somethig
        blog(LOG_ERROR, "ditector: error while starting rendering process");
        return;
    }

    obs_source_process_filter_end(user_data->source_handler, 
                                  user_data->effect_handler,
                                  0, 0);
}

static void censor_update(void *data, obs_data_t* settings) { 
    auto* user_data = static_cast<effect_data*>(data);

    // Update the variables from the settings
    user_data->is_active = obs_data_get_bool(settings, ACTIVE_PROPERTY);
}

static obs_properties_t* censor_properties(void *data) {
    auto* properties = obs_properties_create();

    obs_properties_add_bool(properties, 
                            ACTIVE_PROPERTY, 
                            "True if the effect is active, false if not");
    return properties;
}

static void censor_defaults(obs_data_t *settings) {

}

static const char* censor_get_name(void* data) {
    return "Ditector";
}

obs_source_info* register_effect() {
    obs_source_info* effect_source = new obs_source_info;
    effect_source->id = "ditector_censor_effect",
    effect_source->type = OBS_SOURCE_TYPE_FILTER,
    effect_source->output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CAP_OBSOLETE,
    effect_source->get_name = censor_get_name,
    effect_source->create = censor_create,
    effect_source->destroy = censor_destroy,
    effect_source->video_render = censor_render,
    effect_source->update = censor_update,
    effect_source->get_properties = censor_properties,
    effect_source->get_defaults = censor_defaults,
    
    obs_register_source_s(effect_source, sizeof(obs_source_info));

    return effect_source;
};