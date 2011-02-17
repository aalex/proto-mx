#include <clutter/clutter.h>

gboolean state_matches_string(ClutterState *state, gchar *name);
ClutterScript *require_script(const gchar *filename);
GObject *require_object_from_script(ClutterScript *script, const gchar *name);
