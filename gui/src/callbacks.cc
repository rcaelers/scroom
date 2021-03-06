#include "callbacks.hh"

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#include <cairo.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glade/glade.h>

#include <string>
#include <list>

#include <workinterface.hh>

#include "view.hh"
#include "pluginmanager.hh"
#include "loader.hh"

static const std::string SCROOM_DEV_MODE="SCROOM_DEV_MODE";

static std::string xmlFileName;
static GladeXML* aboutDialogXml=NULL;
static GtkWidget* aboutDialog=NULL;

static std::list<View*> views;
static std::list<PresentationInterface::WeakPtr> presentations;
static std::list<std::string> filenames;

void on_scroom_hide (GtkWidget* widget, gpointer user_data)
{
  // printf("hide\n");
  View* view = static_cast<View*>(user_data);
  on_view_destroyed(view);

  if(views.empty())
    gtk_main_quit();
}

void on_new_activate (GtkMenuItem* menuitem, gpointer user_data)
{
  NewInterface* newInterface = static_cast<NewInterface*>(user_data);
  create(newInterface);
}

void on_open_activate (GtkMenuItem* menuitem, gpointer user_data)
{
  GtkWidget* dialog;
  GtkWidget* scroom = static_cast<GtkWidget*>(user_data);

  printf("Creating the open dialog\n");
  dialog = gtk_file_chooser_dialog_new ("Open File",
                                        GTK_WINDOW(scroom),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                        NULL);
  const std::map<OpenInterface*, std::string>& openInterfaces = PluginManager::getInstance().getOpenInterfaces();

  for(std::map<OpenInterface*, std::string>::const_iterator cur=openInterfaces.begin();
      cur != openInterfaces.end();
      cur++)
  {
    std::list<GtkFileFilter*> filters = cur->first->getFilters();
    for(std::list<GtkFileFilter*>::iterator f = filters.begin();
        f != filters.end();
        f++)
    {
#if MUTRACX_HACKS
      GtkFileFilterFlags flags = gtk_file_filter_get_needed(*f);
      if(flags & ~(GTK_FILE_FILTER_FILENAME | GTK_FILE_FILTER_DISPLAY_NAME))
      {
        printf("ERROR: Only pattern matching is supported (%d, %s)\n", flags, cur->second.c_str());
      }
      else
      {
        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), *f);
      }
#else
      gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), *f);
#endif
      
    }
  }

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
#if MUTRACX_HACKS
    GtkFileFilterInfo filterInfo;
    filterInfo.filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
    filterInfo.display_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
    filterInfo.contains =
      (GtkFileFilterFlags)(GTK_FILE_FILTER_FILENAME | GTK_FILE_FILTER_DISPLAY_NAME);
    printf("Opening file %s (%s)\n", filterInfo.filename, filterInfo.mime_type);
#else
    GFile* file = g_file_new_for_path(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog)));
    GFileInfo* fileInfo = g_file_query_info(file, "standard::*", G_FILE_QUERY_INFO_NONE, NULL, NULL);
    GtkFileFilterInfo filterInfo;
    filterInfo.filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
    filterInfo.mime_type = g_content_type_get_mime_type(g_file_info_get_content_type (fileInfo));
    filterInfo.display_name = g_file_info_get_display_name(fileInfo);
    filterInfo.contains =
      (GtkFileFilterFlags)(GTK_FILE_FILTER_FILENAME | GTK_FILE_FILTER_DISPLAY_NAME | GTK_FILE_FILTER_MIME_TYPE);
    printf("Opening file %s\n", filterInfo.filename);
#endif
    
    load(filterInfo);
  }
  gtk_widget_destroy (dialog);
}

void on_save_activate (GtkMenuItem* menuitem, gpointer user_data)
{

}

void on_save_as_activate (GtkMenuItem* menuitem, gpointer user_data)
{

}

void on_quit_activate (GtkMenuItem* menuitem, gpointer user_data)
{
  gtk_main_quit();
}

void on_cut_activate (GtkMenuItem* menuitem, gpointer user_data)
{

}

void on_copy_activate (GtkMenuItem* menuitem, gpointer user_data)
{

}

void on_paste_activate (GtkMenuItem* menuitem, gpointer user_data)
{

}

void on_delete_activate (GtkMenuItem* menuitem, gpointer user_data)
{

}

void on_about_activate (GtkMenuItem* menuitem, gpointer user_data)
{
  // GtkWidget* aboutdialog;
  // aboutdialog = create_aboutdialog ();
  gtk_dialog_run (GTK_DIALOG (aboutDialog ));
  gtk_widget_hide(aboutDialog);
  // gtk_widget_destroy (aboutdialog);
}

gboolean on_drawingarea_expose_event (GtkWidget* widget, GdkEventExpose* event, gpointer user_data)
{
  // printf("expose\n");

  cairo_t* cr = gdk_cairo_create(widget->window);
  View* view = static_cast<View*>(user_data);
  view->redraw(cr);

  cairo_destroy(cr);
  return FALSE;
}

gboolean on_drawingarea_configure_event (GtkWidget* widget, GdkEventConfigure* event, gpointer user_data)
{
  // printf("configure\n");
  View* view = static_cast<View*>(user_data);
  view->on_configure();
  return FALSE;
}

gboolean on_idle (gpointer user_data)
{
  if(user_data==NULL)
  {
    return 0;
  }
  else
  {
    return reinterpret_cast<WorkInterface*>(user_data)->doWork();
  }
}

void on_done_loading_plugins()
{
  while(!filenames.empty())
  {
    load(filenames.front());
    filenames.pop_front();
  }
}

void on_zoombox_changed(GtkComboBox* widget, gpointer user_data)
{
  View* view = static_cast<View*>(user_data);
  view->on_zoombox_changed();
}

void on_scrollbar_value_changed(GtkAdjustment* adjustment, gpointer user_data)
{
  View* view = static_cast<View*>(user_data);
  view->on_scrollbar_value_changed(adjustment);
}

gboolean on_button_press_event(GtkWidget* widget, GdkEventButton* event, gpointer user_data)
{
  View* view = static_cast<View*>(user_data);
  view->on_buttonPress(event);
  return true;
}

gboolean on_button_release_event(GtkWidget* widget, GdkEventButton* event, gpointer user_data)
{
  View* view = static_cast<View*>(user_data);
  view->on_buttonRelease(event);
  return true;
}

gboolean on_motion_notify_event(GtkWidget* widget, GdkEventMotion* event, gpointer user_data)
{
  View* view = static_cast<View*>(user_data);
  view->on_motion_notify(event);
  return true;
}

gboolean on_scroll_event(GtkWidget* widget, GdkEventScroll* event, gpointer user_data)
{
  View* view = static_cast<View*>(user_data);
  view->on_scrollwheel(event);
  return true;
}

void on_scroom_bootstrap (const std::list<std::string>& newFilenames)
{
  printf("Bootstrapping Scroom...\n");
  filenames = newFilenames;

  bool devMode = NULL!=getenv(SCROOM_DEV_MODE.c_str());
  if(devMode)
  {
    printf("+----------------------------------------------------------------------+\n"
           "| ENTERING DEVELOPMENT MODE                                            |\n"
           "| All the default directories are not searched                         |\n"
           "| Instead, only environment variables and the local source tree        |\n"
           "| are consulted.                                                       |\n"
           "+----------------------------------------------------------------------+\n"
           );
  }
  
  startPluginManager(devMode);

#if defined(WIN32)
  gchar *installDir = g_win32_get_package_installation_directory_of_module(NULL);
  xmlFileName = std::string(installDir) + "\\bin\\scroom.glade";
#else    
  if(devMode)
  { 
    xmlFileName = TOP_SRCDIR "/gui/scroom.glade";
  }
  else
  {
    xmlFileName = PACKAGE_DATA_DIR "/scroom.glade";
  }
#endif  
  aboutDialogXml = glade_xml_new(xmlFileName.c_str(), "aboutDialog", NULL);
  if(aboutDialogXml!=NULL)
  {
    aboutDialog = glade_xml_get_widget(aboutDialogXml, "aboutDialog");
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(aboutDialog), PACKAGE_NAME);
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(aboutDialog), PACKAGE_VERSION);
  }
  else
  {
    printf("Opening xml failed\n");
    exit(-1);
  }

  if(filenames.empty())
  {
    create_scroom(PresentationInterface::Ptr());
  }
}

void find_or_create_scroom(PresentationInterface::Ptr presentation)
{
  for(std::list<View*>::iterator cur = views.begin(); cur != views.end(); cur++)
  {
    if(!(*cur)->hasPresentation())
    {
      (*cur)->setPresentation(presentation);
      return;
    }
  }
  create_scroom(presentation);
}

void create_scroom(PresentationInterface::Ptr presentation)
{
  GladeXML* xml = glade_xml_new(xmlFileName.c_str(), "scroom", NULL);

  if(xml==NULL)
  {
    printf("Opening xml failed\n");
    exit(-1);
  }

  View* view = new View(xml, presentation);
  on_view_created(view);

  GtkWidget* scroom = glade_xml_get_widget(xml, "scroom");
  GtkWidget* newMenuItem = glade_xml_get_widget(xml, "new");
  GtkWidget* openMenuItem = glade_xml_get_widget(xml, "open");
  GtkWidget* quitMenuItem = glade_xml_get_widget(xml, "quit");
  GtkWidget* aboutMenuItem = glade_xml_get_widget(xml, "about");
  GtkWidget* drawingArea = glade_xml_get_widget(xml, "drawingarea");
  GtkWidget* zoomBox = glade_xml_get_widget(xml, "zoomboxcombo");
  GtkWidget* vscrollbar = glade_xml_get_widget(xml, "vscrollbar");
  GtkWidget* hscrollbar = glade_xml_get_widget(xml, "hscrollbar");
  GtkAdjustment* vscrollbaradjustment = gtk_range_get_adjustment(GTK_RANGE(vscrollbar));
  GtkAdjustment* hscrollbaradjustment = gtk_range_get_adjustment(GTK_RANGE(hscrollbar));

  g_signal_connect ((gpointer) scroom, "hide", G_CALLBACK (on_scroom_hide), view);
  // g_signal_connect ((gpointer) open, "activate",
  //                   G_CALLBACK (on_open_activate),
  //                   view);
  // g_signal_connect ((gpointer) save, "activate",
  //                   G_CALLBACK (on_save_activate),
  //                   view);
  // g_signal_connect ((gpointer) save_as, "activate",
  //                   G_CALLBACK (on_save_as_activate),
  //                   view);
  // g_signal_connect ((gpointer) newMenuItem, "activate", G_CALLBACK (on_new_activate), view);
  g_signal_connect ((gpointer) quitMenuItem, "activate", G_CALLBACK (on_quit_activate), view);
  g_signal_connect ((gpointer) openMenuItem, "activate", G_CALLBACK (on_open_activate), scroom);
  g_signal_connect ((gpointer) zoomBox, "changed", G_CALLBACK (on_zoombox_changed), view);
  g_signal_connect ((gpointer) vscrollbaradjustment, "value-changed", G_CALLBACK(on_scrollbar_value_changed), view);
  g_signal_connect ((gpointer) hscrollbaradjustment, "value-changed", G_CALLBACK(on_scrollbar_value_changed), view);
  // g_signal_connect ((gpointer) cut, "activate",
  //                   G_CALLBACK (on_cut_activate),
  //                   view);
  // g_signal_connect ((gpointer) copy, "activate",
  //                   G_CALLBACK (on_copy_activate),
  //                   view);
  // g_signal_connect ((gpointer) paste, "activate",
  //                   G_CALLBACK (on_paste_activate),
  //                   view);
  // g_signal_connect ((gpointer) delete, "activate",
  //                   G_CALLBACK (on_delete_activate),
  //                   view);
  g_signal_connect ((gpointer) aboutMenuItem, "activate", G_CALLBACK (on_about_activate), view);
  g_signal_connect ((gpointer) drawingArea, "expose_event", G_CALLBACK (on_drawingarea_expose_event), view);
  g_signal_connect ((gpointer) drawingArea, "configure_event", G_CALLBACK (on_drawingarea_configure_event), view);
  g_signal_connect ((gpointer) drawingArea, "button-press-event", G_CALLBACK (on_button_press_event), view);
  g_signal_connect ((gpointer) drawingArea, "button-release-event", G_CALLBACK (on_button_release_event), view);
  g_signal_connect ((gpointer) drawingArea, "scroll-event", G_CALLBACK (on_scroll_event), view);
  g_signal_connect ((gpointer) drawingArea, "motion-notify-event", G_CALLBACK (on_motion_notify_event), view);
}

void on_newInterfaces_update(const std::map<NewInterface*, std::string>& newInterfaces)
{
  for(std::list<View*>::iterator cur = views.begin(); cur != views.end(); cur++)
  {
    (*cur)->on_newInterfaces_update(newInterfaces);
  }
}

void on_presentation_created(PresentationInterface::Ptr p)
{
  presentations.push_back(p);

  for(std::list<View*>::iterator cur = views.begin(); cur != views.end(); cur++)
  {
    (*cur)->on_presentation_created(p);
  }

  const std::map<PresentationObserver*, std::string>& presentationObservers =
    PluginManager::getInstance().getPresentationObservers();

  std::map<PresentationObserver*, std::string>::const_iterator cur = presentationObservers.begin();
  std::map<PresentationObserver*, std::string>::const_iterator end = presentationObservers.end();
  for(;cur!=end; cur++)
    cur->first->presentationAdded(p);
}

void on_view_created(View* v)
{
  views.push_back(v);

  for(std::list<PresentationInterface::WeakPtr>::iterator cur = presentations.begin();
      cur != presentations.end(); cur++)
  {
    PresentationInterface::Ptr p = cur->lock();
    if(p)
    {
      v->on_presentation_created(p);
    }
  }

  const std::map<ViewObserver*, std::string>& viewObservers =
    PluginManager::getInstance().getViewObservers();

  std::map<ViewObserver*, std::string>::const_iterator cur = viewObservers.begin();
  std::map<ViewObserver*, std::string>::const_iterator end = viewObservers.end();
  for(;cur!=end; cur++)
    cur->first->viewAdded(v);
}

void on_view_destroyed(View* v)
{
  {
    const std::map<ViewObserver*, std::string>& viewObservers =
      PluginManager::getInstance().getViewObservers();

    std::map<ViewObserver*, std::string>::const_iterator cur = viewObservers.begin();
    std::map<ViewObserver*, std::string>::const_iterator end = viewObservers.end();
    for(;cur!=end; cur++)
      cur->first->viewDeleted(v);
  }
  
  views.remove(v);
  delete v;

  bool presentationDestroyed = false;
  for(std::list<PresentationInterface::WeakPtr>::iterator cur = presentations.begin();
      cur != presentations.end(); cur++)
  {
    PresentationInterface::Ptr p = cur->lock();
    if(!p)
    {
      presentationDestroyed = true;
      std::list<PresentationInterface::WeakPtr>::iterator temp = cur;
      temp--;
      presentations.erase(cur);
      cur = temp;
    }
  }
  if(presentationDestroyed)
  {
    for(std::list<View*>::iterator cur = views.begin(); cur != views.end(); cur++)
    {
      (*cur)->on_presentation_destroyed();
    }

    const std::map<PresentationObserver*, std::string>& presentationObservers =
      PluginManager::getInstance().getPresentationObservers();

    std::map<PresentationObserver*, std::string>::const_iterator cur = presentationObservers.begin();
    std::map<PresentationObserver*, std::string>::const_iterator end = presentationObservers.end();
    for(;cur!=end; cur++)
      cur->first->presentationDeleted();
  }
}

void on_new_presentationobserver(PresentationObserver* po)
{
  for(std::list<PresentationInterface::WeakPtr>::iterator cur = presentations.begin();
      cur != presentations.end(); cur++)
  {
    PresentationInterface::Ptr p = cur->lock();
    if(p)
    {
      po->presentationAdded(p);
    }
  }
}

void on_new_viewobserver(ViewObserver* v)
{
  for(std::list<View*>::iterator cur = views.begin(); cur != views.end(); cur++)
  {
    v->viewAdded(*cur);
  }
}
  
