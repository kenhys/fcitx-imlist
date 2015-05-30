#include <stdio.h>
#include <stdlib.h>
#include <fcitx-gclient/fcitxinputmethod.h>

static gchar *list;
static gchar *setlist;
static gboolean verbose;

static GOptionEntry entries[] = {
  { "list", 'l', 0, G_OPTION_ARG_NONE, &list,
    "List input method", NULL },
  { "set", 's', 0, G_OPTION_ARG_STRING, &setlist,
    "Set input method list separated by ',' for multiple IM", NULL },
  { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose,
    "Be verbose", NULL },
  { NULL }
};

static FcitxInputMethod *im;

static
FcitxInputMethod *
get_fcitx_im(void)
{
  GError* error = NULL;
  FcitxInputMethod *im;

  im = fcitx_input_method_new(G_BUS_TYPE_SESSION,
                              G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES,
                              fcitx_utils_get_display_number(),
                              NULL,
                              &error);
  if (!im) {
    g_error_free(error);
    return;
  }
  return im;
}

int main(int argc, char *argv[])
{
  GPtrArray *im_list;
  GOptionContext *context;
  GError *error;

  context = g_option_context_new("- fcitx input method list utility");
  g_option_context_set_help_enabled(context, TRUE);
  g_option_context_add_main_entries(context, entries, "");
  if (!g_option_context_parse(context, &argc, &argv, &error)) {
    g_print("option parsing failed: %s\n", error->message);
    exit(1);
  }

  if (list || setlist || verbose) {
  } else {
    g_print("%s", g_option_context_get_help(context, FALSE, NULL));
  }

  return 0;
}
