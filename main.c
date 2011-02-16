/**
 * Mx prototype
 */
#include <clutter/clutter.h>
#include <mx/mx.h>

#define GUI_SCRIPT "mockup.json"
#define UNUSED(x) ((void) (x))

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

static const gint WIN_W = 640;
static const gint WIN_H = 480;

typedef struct assistant_
{
    ClutterActor *stage;
    ClutterActor *slider;
    ClutterActor *combo_box;
    ClutterScript *script;
    gboolean ready_;
} Assistant;

static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event, gpointer data)
{
    Assistant *assistant = (Assistant *) data;
    (void) assistant; // unused
    switch (event->keyval)
    {
        case CLUTTER_Escape:
            clutter_main_quit();
            break;
        case CLUTTER_space:
            break;
        default:
            break;
    }
}

//extern "C"
//{
//    void on_button_clicked(MxButton *button, gpointer user_data);
//}

void on_button_clicked(MxButton *button, gpointer user_data)
{
    Assistant *assistant = (Assistant *) user_data;
    (void) assistant; // unused
    (void) button; // unused
    g_print("clicked\n");
    if (assistant->ready_)
    {
        g_print("slider value: %f\n", mx_slider_get_value(MX_SLIDER(assistant->slider)));
    }
}

int main(int argc, char *argv[])
{
    clutter_init(&argc, &argv);

    ClutterActor *stage = NULL;
    ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
    stage = clutter_stage_get_default();
    clutter_stage_set_title(CLUTTER_STAGE(stage), "Mx test");
    clutter_stage_set_color(CLUTTER_STAGE(stage), &black);
    clutter_actor_set_size(stage, WIN_W, WIN_H);

    Assistant *assistant = g_new0(Assistant, 1);
    assistant->script = require_script(GUI_SCRIPT);
    assistant->stage = stage;
    ClutterActor *root = CLUTTER_ACTOR(require_object_from_script(assistant->script, "root"));
    assistant->slider = CLUTTER_ACTOR(require_object_from_script(assistant->script, "slider"));
    assistant->combo_box = CLUTTER_ACTOR(require_object_from_script(assistant->script, "combo_box"));
    clutter_container_add_actor(CLUTTER_CONTAINER(stage), root);

    // Combo box contents:
    MxComboBox *combo_box = MX_COMBO_BOX(assistant->combo_box);
    mx_combo_box_append_text(combo_box, "Foo");
    mx_combo_box_append_text(combo_box, "Spam");
    mx_combo_box_append_text(combo_box, "Lorem ipsum");
    mx_combo_box_set_index(combo_box, 0);

    // DONE
    g_signal_connect(stage, "key-press-event", G_CALLBACK(key_event_cb), assistant);
    clutter_script_connect_signals(assistant->script, assistant);
    assistant->ready_ = TRUE;
    clutter_actor_show(stage);
    clutter_main();
    
    UNUSED(on_button_clicked);
    return 0;
}

