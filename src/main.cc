/*
 * This project is provided under the GNU GPL v2 license, more information can 
 * found on https://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    - Iker Galardi
 */

#include <exception>

#include <obs/obs-module.h>

#include "pinfo.hh"
#include "effect.hh"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

obs_source_info* g_source_info;

bool obs_module_load() {

    try {
        g_source_info = register_effect();
    } catch (const std::exception& e) {
        blog(LOG_ERROR, "[ditector] error loading the effect: %s", e.what());

        // Delete if source has been created
        if(g_source_info != nullptr)
            delete g_source_info;

        return false;
    }

    blog(LOG_INFO, "[ditector] plugin loaded successfully (version %s)", PLUGIN_VERSION);

    return true;
}

void obs_module_unload() {
    // Delete if source has been created
    if(g_source_info != nullptr)
        delete g_source_info;

    blog(LOG_INFO, "[ditector] plugin unloaded");
}
