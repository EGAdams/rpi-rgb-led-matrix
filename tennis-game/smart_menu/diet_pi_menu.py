from MenuManager import MenuManager
# from Menu import Menu
from Menu import Menu
if __name__ == "__main__":
    config_path = "/home/eg1972/linuxBash/python_menus/smart_menu/diet_pi_menu_config.json"

    menu = Menu()
    menu_manager = MenuManager(menu, config_path )
    menu_manager.load_menus()
    menu.display_and_select(menu_manager)
