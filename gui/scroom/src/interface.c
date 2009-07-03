/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_scroom (void)
{
  GtkWidget *scroom;
  GtkWidget *vbox1;
  GtkWidget *menubar1;
  GtkWidget *file_menu_item;
  GtkWidget *file_menu;
  GtkWidget *new;
  GtkWidget *open;
  GtkWidget *save;
  GtkWidget *save_as;
  GtkWidget *separatormenuitem1;
  GtkWidget *quit;
  GtkWidget *edit_menu_item;
  GtkWidget *edit_menu;
  GtkWidget *cut;
  GtkWidget *copy;
  GtkWidget *paste;
  GtkWidget *delete;
  GtkWidget *view_menu_item;
  GtkWidget *view_menu;
  GtkWidget *help_menu_item;
  GtkWidget *help_menu;
  GtkWidget *about;
  GtkWidget *toolbar1;
  GtkIconSize tmp_toolbar_icon_size;
  GtkWidget *toolitem2;
  GtkWidget *filechooserbutton1;
  GtkWidget *zoombox;
  GtkWidget *combobox1;
  GtkWidget *table1;
  GtkWidget *drawingarea;
  GtkWidget *hscrollbar;
  GtkWidget *vscrollbar;
  GtkWidget *vruler;
  GtkWidget *hruler;
  GtkWidget *hbox2;
  GtkWidget *progressbar;
  GtkWidget *statusbar1;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  scroom = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (scroom), _("Scroom"));

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (scroom), vbox1);

  menubar1 = gtk_menu_bar_new ();
  gtk_widget_show (menubar1);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

  file_menu_item = gtk_menu_item_new_with_mnemonic (_("_File"));
  gtk_widget_show (file_menu_item);
  gtk_container_add (GTK_CONTAINER (menubar1), file_menu_item);

  file_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_menu_item), file_menu);

  new = gtk_image_menu_item_new_from_stock ("gtk-new", accel_group);
  gtk_widget_show (new);
  gtk_container_add (GTK_CONTAINER (file_menu), new);

  open = gtk_image_menu_item_new_from_stock ("gtk-open", accel_group);
  gtk_widget_show (open);
  gtk_container_add (GTK_CONTAINER (file_menu), open);

  save = gtk_image_menu_item_new_from_stock ("gtk-save", accel_group);
  gtk_widget_show (save);
  gtk_container_add (GTK_CONTAINER (file_menu), save);

  save_as = gtk_image_menu_item_new_from_stock ("gtk-save-as", accel_group);
  gtk_widget_show (save_as);
  gtk_container_add (GTK_CONTAINER (file_menu), save_as);

  separatormenuitem1 = gtk_separator_menu_item_new ();
  gtk_widget_show (separatormenuitem1);
  gtk_container_add (GTK_CONTAINER (file_menu), separatormenuitem1);
  gtk_widget_set_sensitive (separatormenuitem1, FALSE);

  quit = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_show (quit);
  gtk_container_add (GTK_CONTAINER (file_menu), quit);

  edit_menu_item = gtk_menu_item_new_with_mnemonic (_("_Edit"));
  gtk_widget_show (edit_menu_item);
  gtk_container_add (GTK_CONTAINER (menubar1), edit_menu_item);

  edit_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (edit_menu_item), edit_menu);

  cut = gtk_image_menu_item_new_from_stock ("gtk-cut", accel_group);
  gtk_widget_show (cut);
  gtk_container_add (GTK_CONTAINER (edit_menu), cut);

  copy = gtk_image_menu_item_new_from_stock ("gtk-copy", accel_group);
  gtk_widget_show (copy);
  gtk_container_add (GTK_CONTAINER (edit_menu), copy);

  paste = gtk_image_menu_item_new_from_stock ("gtk-paste", accel_group);
  gtk_widget_show (paste);
  gtk_container_add (GTK_CONTAINER (edit_menu), paste);

  delete = gtk_image_menu_item_new_from_stock ("gtk-delete", accel_group);
  gtk_widget_show (delete);
  gtk_container_add (GTK_CONTAINER (edit_menu), delete);

  view_menu_item = gtk_menu_item_new_with_mnemonic (_("_View"));
  gtk_widget_show (view_menu_item);
  gtk_container_add (GTK_CONTAINER (menubar1), view_menu_item);

  view_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (view_menu_item), view_menu);

  help_menu_item = gtk_menu_item_new_with_mnemonic (_("_Help"));
  gtk_widget_show (help_menu_item);
  gtk_container_add (GTK_CONTAINER (menubar1), help_menu_item);

  help_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (help_menu_item), help_menu);

  about = gtk_menu_item_new_with_mnemonic (_("_About"));
  gtk_widget_show (about);
  gtk_container_add (GTK_CONTAINER (help_menu), about);

  toolbar1 = gtk_toolbar_new ();
  gtk_widget_show (toolbar1);
  gtk_box_pack_start (GTK_BOX (vbox1), toolbar1, FALSE, FALSE, 0);
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar1), GTK_TOOLBAR_BOTH);
  tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1));

  toolitem2 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem2);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem2);

  filechooserbutton1 = gtk_file_chooser_button_new (_("Select A File"), GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_widget_show (filechooserbutton1);
  gtk_container_add (GTK_CONTAINER (toolitem2), filechooserbutton1);

  zoombox = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (zoombox);
  gtk_container_add (GTK_CONTAINER (toolbar1), zoombox);

  combobox1 = gtk_combo_box_new_text ();
  gtk_widget_show (combobox1);
  gtk_container_add (GTK_CONTAINER (zoombox), combobox1);

  table1 = gtk_table_new (3, 3, FALSE);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (vbox1), table1, TRUE, TRUE, 0);

  drawingarea = gtk_drawing_area_new ();
  gtk_widget_show (drawingarea);
  gtk_table_attach (GTK_TABLE (table1), drawingarea, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  hscrollbar = gtk_hscrollbar_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 0, 0, 0, 0)));
  gtk_widget_show (hscrollbar);
  gtk_table_attach (GTK_TABLE (table1), hscrollbar, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  vscrollbar = gtk_vscrollbar_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 0, 0, 0, 0)));
  gtk_widget_show (vscrollbar);
  gtk_table_attach (GTK_TABLE (table1), vscrollbar, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  vruler = gtk_vruler_new ();
  gtk_widget_show (vruler);
  gtk_table_attach (GTK_TABLE (table1), vruler, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_ruler_set_range (GTK_RULER (vruler), 0, 100, 6.05505, 100);

  hruler = gtk_hruler_new ();
  gtk_widget_show (hruler);
  gtk_table_attach (GTK_TABLE (table1), hruler, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_ruler_set_range (GTK_RULER (hruler), 0, 10, 0.395833, 10);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox2, FALSE, FALSE, 0);

  progressbar = gtk_progress_bar_new ();
  gtk_widget_show (progressbar);
  gtk_box_pack_start (GTK_BOX (hbox2), progressbar, FALSE, FALSE, 0);

  statusbar1 = gtk_statusbar_new ();
  gtk_widget_show (statusbar1);
  gtk_box_pack_start (GTK_BOX (hbox2), statusbar1, FALSE, FALSE, 0);

  g_signal_connect ((gpointer) new, "activate",
                    G_CALLBACK (on_new_activate),
                    NULL);
  g_signal_connect ((gpointer) open, "activate",
                    G_CALLBACK (on_open_activate),
                    NULL);
  g_signal_connect ((gpointer) save, "activate",
                    G_CALLBACK (on_save_activate),
                    NULL);
  g_signal_connect ((gpointer) save_as, "activate",
                    G_CALLBACK (on_save_as_activate),
                    NULL);
  g_signal_connect ((gpointer) quit, "activate",
                    G_CALLBACK (on_quit_activate),
                    NULL);
  g_signal_connect ((gpointer) cut, "activate",
                    G_CALLBACK (on_cut_activate),
                    NULL);
  g_signal_connect ((gpointer) copy, "activate",
                    G_CALLBACK (on_copy_activate),
                    NULL);
  g_signal_connect ((gpointer) paste, "activate",
                    G_CALLBACK (on_paste_activate),
                    NULL);
  g_signal_connect ((gpointer) delete, "activate",
                    G_CALLBACK (on_delete_activate),
                    NULL);
  g_signal_connect ((gpointer) about, "activate",
                    G_CALLBACK (on_about_activate),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (scroom, scroom, "scroom");
  GLADE_HOOKUP_OBJECT (scroom, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (scroom, menubar1, "menubar1");
  GLADE_HOOKUP_OBJECT (scroom, file_menu_item, "file_menu_item");
  GLADE_HOOKUP_OBJECT (scroom, file_menu, "file_menu");
  GLADE_HOOKUP_OBJECT (scroom, new, "new");
  GLADE_HOOKUP_OBJECT (scroom, open, "open");
  GLADE_HOOKUP_OBJECT (scroom, save, "save");
  GLADE_HOOKUP_OBJECT (scroom, save_as, "save_as");
  GLADE_HOOKUP_OBJECT (scroom, separatormenuitem1, "separatormenuitem1");
  GLADE_HOOKUP_OBJECT (scroom, quit, "quit");
  GLADE_HOOKUP_OBJECT (scroom, edit_menu_item, "edit_menu_item");
  GLADE_HOOKUP_OBJECT (scroom, edit_menu, "edit_menu");
  GLADE_HOOKUP_OBJECT (scroom, cut, "cut");
  GLADE_HOOKUP_OBJECT (scroom, copy, "copy");
  GLADE_HOOKUP_OBJECT (scroom, paste, "paste");
  GLADE_HOOKUP_OBJECT (scroom, delete, "delete");
  GLADE_HOOKUP_OBJECT (scroom, view_menu_item, "view_menu_item");
  GLADE_HOOKUP_OBJECT (scroom, view_menu, "view_menu");
  GLADE_HOOKUP_OBJECT (scroom, help_menu_item, "help_menu_item");
  GLADE_HOOKUP_OBJECT (scroom, help_menu, "help_menu");
  GLADE_HOOKUP_OBJECT (scroom, about, "about");
  GLADE_HOOKUP_OBJECT (scroom, toolbar1, "toolbar1");
  GLADE_HOOKUP_OBJECT (scroom, toolitem2, "toolitem2");
  GLADE_HOOKUP_OBJECT (scroom, filechooserbutton1, "filechooserbutton1");
  GLADE_HOOKUP_OBJECT (scroom, zoombox, "zoombox");
  GLADE_HOOKUP_OBJECT (scroom, combobox1, "combobox1");
  GLADE_HOOKUP_OBJECT (scroom, table1, "table1");
  GLADE_HOOKUP_OBJECT (scroom, drawingarea, "drawingarea");
  GLADE_HOOKUP_OBJECT (scroom, hscrollbar, "hscrollbar");
  GLADE_HOOKUP_OBJECT (scroom, vscrollbar, "vscrollbar");
  GLADE_HOOKUP_OBJECT (scroom, vruler, "vruler");
  GLADE_HOOKUP_OBJECT (scroom, hruler, "hruler");
  GLADE_HOOKUP_OBJECT (scroom, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (scroom, progressbar, "progressbar");
  GLADE_HOOKUP_OBJECT (scroom, statusbar1, "statusbar1");

  gtk_window_add_accel_group (GTK_WINDOW (scroom), accel_group);

  return scroom;
}

GtkWidget*
create_aboutdialog (void)
{
  GtkWidget *aboutdialog;
  /* TRANSLATORS: Replace this string with your names, one name per line. */
  gchar *translators = _("translator-credits");

  aboutdialog = gtk_about_dialog_new ();
  gtk_container_set_border_width (GTK_CONTAINER (aboutdialog), 5);
  gtk_window_set_destroy_with_parent (GTK_WINDOW (aboutdialog), TRUE);
  gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (aboutdialog), VERSION);
  gtk_about_dialog_set_name (GTK_ABOUT_DIALOG (aboutdialog), _("Scroom"));
  gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (aboutdialog), _("(C)2009 Kees-Jan Dijkzeul"));
  gtk_about_dialog_set_wrap_license (GTK_ABOUT_DIALOG (aboutdialog), TRUE);
  gtk_about_dialog_set_translator_credits (GTK_ABOUT_DIALOG (aboutdialog), translators);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (aboutdialog, aboutdialog, "aboutdialog");

  return aboutdialog;
}

