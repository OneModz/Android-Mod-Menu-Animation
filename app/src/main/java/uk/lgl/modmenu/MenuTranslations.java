package uk.lgl.modmenu;

public final class MenuTranslations {

    public static final int ENGLISH = 0;
    public static final int PORTUGUESE = 1;
    public static final int SPANISH = 2;

    private MenuTranslations() {
    }

    public static String get(int language, String key) {

        switch (language) {

            case PORTUGUESE:
                return pt(key);

            case SPANISH:
                return es(key);

            case ENGLISH:
            default:
                return en(key);
        }
    }

    private static String en(String key) {

        switch (key) {

            case "subtitle":
                return "Standalone Sandbox";

            case "hide_kill":
                return "HIDE / KILL";

            case "minimize":
                return "MINIMIZE";

            case "basic_title":
                return "Basic settings";

            case "visual_options":
                return "Visual options";

            case "show_lines":
                return "Show lines";

            case "show_lines_desc":
                return "Display helper lines";

            case "keep_lines":
                return "Keep lines after shot";

            case "keep_lines_desc":
                return "Keep helper lines visible";

            case "lines_title":
                return "Line style";

            case "appearance":
                return "Appearance";

            case "style":
                return "Style";

            case "solid":
                return "Solid";

            case "dashed":
                return "Dashed";

            case "dotted":
                return "Dotted";

            case "line_width":
                return "Line width";

            case "line_opacity":
                return "Line opacity";

            case "language":
                return "Language";

            case "language_desc":
                return "Interface language";

            case "autoplay_title":
                return "Auto play settings";

            case "simulation":
                return "Simulation";

            case "enable_autoplay":
                return "Enable auto play";

            case "enable_autoplay_desc":
                return "Enable automatic sandbox actions";

            case "autoplay_mode":
                return "Auto play mode";

            case "semi_auto":
                return "Semi Auto";

            case "full_auto":
                return "Full Auto";

            case "force":
                return "Force";

            case "action_interval":
                return "Action interval";

            case "autoqueue_title":
                return "Auto queue settings";

            case "queue":
                return "Queue";

            case "enable_autoqueue":
                return "Enable auto queue";

            case "enable_autoqueue_desc":
                return "Enable queue simulation";

            case "queue_mode":
                return "Queue mode";

            case "slow":
                return "Slow";

            case "global":
                return "Global";

            case "fast":
                return "Fast";

            case "coins_to_use":
                return "Coins to use";

            case "filtering":
                return "Filtering";

            case "high_tables":
                return "100k tables or higher only";

            case "mixed_join":
                return "Mixed join";

            case "fix_single_table":
                return "Fix on single table";

            case "table_selection":
                return "Table selection";

            case "simulation_test":
                return "Simulation test";

            case "simulation_test_desc":
                return "Controls the standalone sandbox match";

            case "start_simulation":
                return "Start simulation";

            case "end_simulation":
                return "End simulation";

            default:
                return key;
        }
    }

    private static String pt(String key) {

        switch (key) {

            case "subtitle":
                return "Sandbox independente";

            case "hide_kill":
                return "OCULTAR / ENCERRAR";

            case "minimize":
                return "MINIMIZAR";

            case "basic_title":
                return "Configurações básicas";

            case "visual_options":
                return "Opções visuais";

            case "show_lines":
                return "Mostrar linhas";

            case "show_lines_desc":
                return "Exibir linhas auxiliares";

            case "keep_lines":
                return "Manter linhas após a jogada";

            case "keep_lines_desc":
                return "Manter linhas auxiliares visíveis";

            case "lines_title":
                return "Estilo das linhas";

            case "appearance":
                return "Aparência";

            case "style":
                return "Estilo";

            case "solid":
                return "Sólida";

            case "dashed":
                return "Tracejada";

            case "dotted":
                return "Pontilhada";

            case "line_width":
                return "Espessura da linha";

            case "line_opacity":
                return "Opacidade da linha";

            case "language":
                return "Idioma";

            case "language_desc":
                return "Idioma da interface";

            case "autoplay_title":
                return "Configurações de Auto Play";

            case "simulation":
                return "Simulação";

            case "enable_autoplay":
                return "Ativar Auto Play";

            case "enable_autoplay_desc":
                return "Ativar ações automáticas do sandbox";

            case "autoplay_mode":
                return "Modo do Auto Play";

            case "semi_auto":
                return "Semi Auto";

            case "full_auto":
                return "Auto Completo";

            case "force":
                return "Força";

            case "action_interval":
                return "Intervalo de ação";

            case "autoqueue_title":
                return "Configurações de Auto Queue";

            case "queue":
                return "Fila";

            case "enable_autoqueue":
                return "Ativar Auto Queue";

            case "enable_autoqueue_desc":
                return "Ativar simulação automática de fila";

            case "queue_mode":
                return "Modo da fila";

            case "slow":
                return "Lento";

            case "global":
                return "Global";

            case "fast":
                return "Rápido";

            case "coins_to_use":
                return "Moedas para usar";

            case "filtering":
                return "Filtros";

            case "high_tables":
                return "Somente mesas de 100k ou mais";

            case "mixed_join":
                return "Entrada mista";

            case "fix_single_table":
                return "Fixar em uma única mesa";

            case "table_selection":
                return "Seleção de mesa";

            case "simulation_test":
                return "Teste de simulação";

            case "simulation_test_desc":
                return "Controla a partida do sandbox independente";

            case "start_simulation":
                return "Iniciar simulação";

            case "end_simulation":
                return "Encerrar simulação";

            default:
                return key;
        }
    }

    private static String es(String key) {

        switch (key) {

            case "subtitle":
                return "Sandbox independiente";

            case "hide_kill":
                return "OCULTAR / CERRAR";

            case "minimize":
                return "MINIMIZAR";

            case "basic_title":
                return "Configuración básica";

            case "visual_options":
                return "Opciones visuales";

            case "show_lines":
                return "Mostrar líneas";

            case "show_lines_desc":
                return "Mostrar líneas auxiliares";

            case "keep_lines":
                return "Mantener líneas después del tiro";

            case "keep_lines_desc":
                return "Mantener visibles las líneas auxiliares";

            case "lines_title":
                return "Estilo de líneas";

            case "appearance":
                return "Apariencia";

            case "style":
                return "Estilo";

            case "solid":
                return "Sólida";

            case "dashed":
                return "Discontinua";

            case "dotted":
                return "Punteada";

            case "line_width":
                return "Grosor de línea";

            case "line_opacity":
                return "Opacidad de línea";

            case "language":
                return "Idioma";

            case "language_desc":
                return "Idioma de la interfaz";

            case "autoplay_title":
                return "Configuración de Auto Play";

            case "simulation":
                return "Simulación";

            case "enable_autoplay":
                return "Activar Auto Play";

            case "enable_autoplay_desc":
                return "Activar acciones automáticas del sandbox";

            case "autoplay_mode":
                return "Modo de Auto Play";

            case "semi_auto":
                return "Semi Auto";

            case "full_auto":
                return "Auto Completo";

            case "force":
                return "Fuerza";

            case "action_interval":
                return "Intervalo de acción";

            case "autoqueue_title":
                return "Configuración de Auto Queue";

            case "queue":
                return "Cola";

            case "enable_autoqueue":
                return "Activar Auto Queue";

            case "enable_autoqueue_desc":
                return "Activar simulación automática de cola";

            case "queue_mode":
                return "Modo de cola";

            case "slow":
                return "Lento";

            case "global":
                return "Global";

            case "fast":
                return "Rápido";

            case "coins_to_use":
                return "Monedas a usar";

            case "filtering":
                return "Filtros";

            case "high_tables":
                return "Solo mesas de 100k o más";

            case "mixed_join":
                return "Entrada mixta";

            case "fix_single_table":
                return "Fijar en una sola mesa";

            case "table_selection":
                return "Selección de mesa";

            case "simulation_test":
                return "Prueba de simulación";

            case "simulation_test_desc":
                return "Controla la partida del sandbox independiente";

            case "start_simulation":
                return "Iniciar simulación";

            case "end_simulation":
                return "Finalizar simulación";

            default:
                return key;
        }
    }
}
