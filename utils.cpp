#include <clutter/clutter.h>
#include "utils.h"

gboolean state_matches_string(ClutterState *state, gchar *name)
{
    return g_strcmp0(clutter_state_get_state(state), name) == 0;
}

/**
 * Loads a ClutterScript from a file.
 */
ClutterScript *require_script(const gchar *filename)
{
    GError *error = NULL;
    ClutterScript *script = clutter_script_new();
    clutter_script_load_from_file(script, filename, &error);
    if (error)
    {
        g_error("Error loading JSON script %s: %s", filename, error->message);
        g_error_free(error);
        g_object_unref(script);
        script = NULL;
    }
    return script;
}

/**
 * Loads a GObject from a ClutterScript.
 */
GObject *require_object_from_script(ClutterScript *script, const gchar *name)
{
    GObject *obj = NULL;
    obj = clutter_script_get_object(script, name);
    if (obj == NULL) 
    {
        gboolean filename_set;
        gchar *filename;

        g_object_get(G_OBJECT(script), "filename-set", &filename_set, "filename", &filename, NULL);
        g_error("No object %s found in clutter script %s.", name, filename_set ? filename : "unknown");
        g_free(filename);
    }
    return obj;
}
