#include <stdio.h>
#include <stdlib.h>
#include <fcitx-gclient/fcitxinputmethod.h>

static gchar *list;
static gchar *setlist;
static gboolean verbose;
static gboolean toggle;

static GOptionEntry entries[] = {
  { "list", 'l', 0, G_OPTION_ARG_NONE, &list,
    "List input method", NULL },
  { "set", 's', 0, G_OPTION_ARG_STRING, &setlist,
    "Set input method list separated by ',' for multiple IM", NULL },
  { "toggle", 't', 0, G_OPTION_ARG_NONE, &toggle,
    "Toggle keyboard layout", NULL },
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

void
print_fcitx_imitem_foreach_cb(gpointer data, gpointer user_data)
{
  FcitxIMItem *item = data;
  if (item->enable || verbose) {
    printf("%s (%s) [%s]\n",
           item->unique_name, item->name,
           item->enable ? "enabled" : "disabled");
  }
}

typedef struct _CheckFcitxIMItem {
  gchar *name;
  gboolean exist;
  gboolean abbrev;
} CheckFcitxIMItem;

void
search_fcitx_imitem_foreach_cb(gpointer data, gpointer user_data)
{
  FcitxIMItem *item = data;
  CheckFcitxIMItem *check = user_data;
  gchar *full_name;
  if (item->enable) {
    if (g_strcmp0(item->unique_name, check->name) == 0) {
      check->exist = TRUE;
    } else {
      full_name = g_strdup_printf("fcitx-keyboard-%s", check->name);
      if (g_strcmp0(item->unique_name, full_name) == 0) {
        check->abbrev = TRUE;
      }
      g_free(full_name);
    }
  }
}

void list_input_method(void)
{
  GPtrArray *im_list;
  im = get_fcitx_im();
  if (im) {
    im_list = fcitx_input_method_get_imlist(im);
    g_ptr_array_foreach(im_list, print_fcitx_imitem_foreach_cb, NULL);
    g_object_unref(im);
  }
}

int
fcitx_imitem_compare_func(gconstpointer a,
                          gconstpointer b,
                          gpointer user_data)
{
  FcitxIMItem *item = *(FcitxIMItem **)a;
  FcitxIMItem *item2 = *(FcitxIMItem **)b;
  gint priority_a, priority_b;
  GHashTable *hash = user_data;
  gpointer value;

  priority_a = priority_b = g_hash_table_size(hash) + 1;

  value = g_hash_table_lookup(hash, item->unique_name);
  if (value) {
    priority_a = GPOINTER_TO_SIZE(value);
  }

  value = g_hash_table_lookup(hash, item2->unique_name);
  if (value) {
    priority_b = GPOINTER_TO_SIZE(value);
  }
  return priority_a - priority_b;
}

void toggle_input_method_set(void)
{
  GSettings *settings;
  gint n, n_set;
  gboolean use_default;
  gchar *key, *value;

  settings = g_settings_new("org.xdump.fcitximlist");
  use_default = g_settings_get_boolean(settings, "fcitx-imlist-set");
  if (!use_default) {
    value = g_settings_get_string(settings, "fcitx-imlist-alternative");
  } else {
    value = g_settings_get_string(settings, "fcitx-imlist-default");
  }
  g_settings_set_boolean(settings, "fcitx-imlist-set", !use_default);
  set_input_method_list(value);
  g_free(value);
  g_settings_sync();
}

void set_input_method_list(const gchar *setlist)
{
  gchar **lists;
  gchar **p;
  GHashTable *hash;
  gint priority = 1;
  GPtrArray *im_list;
  CheckFcitxIMItem item;

  lists = g_strsplit(setlist, ",", -1);
  hash = g_hash_table_new(g_str_hash, g_str_equal);
  p = lists;
  im = get_fcitx_im();
  if (im) {
    im_list = fcitx_input_method_get_imlist(im);
    item.exist = FALSE;
    item.abbrev = FALSE;
    for (p = lists; *p; p++, priority++) {
      item.name = *p;
      g_ptr_array_foreach(im_list, search_fcitx_imitem_foreach_cb, &item);
      if (item.exist == FALSE && item.abbrev == TRUE) {
        g_hash_table_insert(hash, g_strdup_printf("fcitx-keyboard-%s", *p), GSIZE_TO_POINTER(priority));
      } else {
        g_hash_table_insert(hash, g_strdup(*p), GSIZE_TO_POINTER(priority));
      }
    }
    g_ptr_array_sort_with_data(im_list, fcitx_imitem_compare_func, hash);
    fcitx_input_method_set_imlist(im, im_list);
    g_ptr_array_foreach(im_list, print_fcitx_imitem_foreach_cb, NULL);
    g_object_unref(im);
  }
  g_hash_table_unref(hash);
  g_strfreev(lists);
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
    if (list) {
      list_input_method();
    } else if (setlist) {
      set_input_method_list(setlist);
    }
  } else if (toggle) {
    toggle_input_method_set();
  } else {
    g_print("%s", g_option_context_get_help(context, FALSE, NULL));
  }

  return 0;
}
