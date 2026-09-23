package uk.lgl.modmenu;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;

import uk.lgl.R;

public class FloatingMenuController {

    private static final int COLOR_ACTIVE = Color.rgb(156, 255, 46);
    private static final int COLOR_NORMAL = Color.WHITE;

    private final Context context;
    private final View root;
    private final FrameLayout container;

    private final Button tabBasic;
    private final Button tabLines;
    private final Button tabAutoPlay;
    private final Button tabAutoQueue;

    /*
     * Idioma atual da interface:
     *
     * 0 = English
     * 1 = Português
     * 2 = Español
     */
    private int currentLanguage =
            MenuTranslations.ENGLISH;

    public FloatingMenuController(Context context, View root) {
        this.context = context;
        this.root = root;

        container = root.findViewById(R.id.tabContainer);

        tabBasic = root.findViewById(R.id.tabBasic);
        tabLines = root.findViewById(R.id.tabLines);
        tabAutoPlay = root.findViewById(R.id.tabAutoPlay);
        tabAutoQueue = root.findViewById(R.id.tabAutoQueue);

        currentLanguage =
                clamp(
                        Preferences.loadPrefInt(
                                "Language",
                                28
                        ),
                        MenuTranslations.ENGLISH,
                        MenuTranslations.SPANISH
                );

        bindSidebar();

        /*
         * Traduz header e footer.
         */
        applyTranslationsToView(
                root,
                currentLanguage
        );

        showBasic();
    }

    private void bindSidebar() {

        tabBasic.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showBasic();
            }
        });

        tabLines.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showLines();
            }
        });

        tabAutoPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showAutoPlay();
            }
        });

        tabAutoQueue.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showAutoQueue();
            }
        });
    }

    private View inflateTab(int layoutRes, Button activeButton) {

        container.removeAllViews();

        View view = LayoutInflater.from(context)
                .inflate(layoutRes, container, false);

        container.addView(view);

        setActiveTab(activeButton);

        /*
         * O XML continua usando inglês como texto-base,
         * mas é traduzido ao ser exibido.
         */
        applyTranslationsToView(
                view,
                currentLanguage
        );

        return view;
    }

    private void setActiveTab(Button activeButton) {

        Button[] buttons = {
                tabBasic,
                tabLines,
                tabAutoPlay,
                tabAutoQueue
        };

        for (Button button : buttons) {

            if (button == activeButton) {
                button.setBackgroundResource(
                        R.drawable.bg_sidebar_button_active
                );

                button.setTextColor(COLOR_ACTIVE);

            } else {

                button.setBackgroundResource(
                        R.drawable.bg_sidebar_button
                );

                button.setTextColor(COLOR_NORMAL);
            }
        }
    }

    /*
     * =========================================================
     * BASIC
     * =========================================================
     */

    private void showBasic() {

        View view = inflateTab(
                R.layout.tab_basic,
                tabBasic
        );

        enableVerticalScroll(
                view,
                R.id.scrollBasic
        );

        final Switch swShowLines =
                view.findViewById(R.id.swShowLines);

        final Switch swKeepLines =
                view.findViewById(R.id.swKeepLines);

        boolean showLines =
                Preferences.loadPrefBool(
                        "Show Lines",
                        19,
                        false
                );

        boolean keepLines =
                Preferences.loadPrefBool(
                        "Keep Lines After Shot",
                        20,
                        false
                );

        swShowLines.setChecked(showLines);
        swKeepLines.setChecked(keepLines);

        swShowLines.setOnCheckedChangeListener(
                (buttonView, isChecked) ->
                        Preferences.changeFeatureBool(
                                "Show Lines",
                                19,
                                isChecked
                        )
        );

        swKeepLines.setOnCheckedChangeListener(
                (buttonView, isChecked) ->
                        Preferences.changeFeatureBool(
                                "Keep Lines After Shot",
                                20,
                                isChecked
                        )
        );
    }

    /*
     * =========================================================
     * LINES
     * =========================================================
     */

    private void showLines() {

        View view = inflateTab(
                R.layout.tab_lines,
                tabLines
        );

        enableVerticalScroll(
                view,
                R.id.scrollLines
        );

        final Spinner spinnerLineStyle =
                view.findViewById(R.id.spinnerLineStyle);

        final Button btnLineSolid =
                view.findViewById(R.id.btnLineSolid);

        final Button btnLineDashed =
                view.findViewById(R.id.btnLineDashed);

        final Button btnLineDotted =
                view.findViewById(R.id.btnLineDotted);

        final SeekBar seekLineWidth =
                view.findViewById(R.id.seekLineWidth);

        final TextView txtLineWidthValue =
                view.findViewById(R.id.txtLineWidthValue);

        final SeekBar seekLineOpacity =
                view.findViewById(R.id.seekLineOpacity);

        final TextView txtLineOpacityValue =
                view.findViewById(R.id.txtLineOpacityValue);

        final Spinner spinnerLanguage =
                view.findViewById(R.id.spinnerLanguage);


        /*
         * LINE STYLE
         *
         * 0 = Solid
         * 1 = Dashed
         * 2 = Dotted
         */

        String[] lineStyles = {
                MenuTranslations.get(
                        currentLanguage,
                        "solid"
                ),
                MenuTranslations.get(
                        currentLanguage,
                        "dashed"
                ),
                MenuTranslations.get(
                        currentLanguage,
                        "dotted"
                )
        };

        ArrayAdapter<String> lineStyleAdapter =
                new ArrayAdapter<>(
                        context,
                        android.R.layout.simple_spinner_item,
                        lineStyles
                );

        lineStyleAdapter.setDropDownViewResource(
                android.R.layout.simple_spinner_dropdown_item
        );

        /*
         * Spinner permanece escondido no XML apenas para
         * compatibilidade estrutural.
         */

        spinnerLineStyle.setAdapter(
                lineStyleAdapter
        );

        int lineStyle =
                clamp(
                        Preferences.loadPrefInt(
                                "Line Style",
                                21
                        ),
                        0,
                        2
                );

        spinnerLineStyle.setSelection(
                lineStyle
        );

        updateLineStyleButtons(
                btnLineSolid,
                btnLineDashed,
                btnLineDotted,
                lineStyle
        );


        btnLineSolid.setOnClickListener(
                new View.OnClickListener() {

                    @Override
                    public void onClick(View v) {

                        Preferences.changeFeatureInt(
                                "Line Style",
                                21,
                                0
                        );

                        spinnerLineStyle.setSelection(
                                0
                        );

                        updateLineStyleButtons(
                                btnLineSolid,
                                btnLineDashed,
                                btnLineDotted,
                                0
                        );
                    }
                }
        );


        btnLineDashed.setOnClickListener(
                new View.OnClickListener() {

                    @Override
                    public void onClick(View v) {

                        Preferences.changeFeatureInt(
                                "Line Style",
                                21,
                                1
                        );

                        spinnerLineStyle.setSelection(
                                1
                        );

                        updateLineStyleButtons(
                                btnLineSolid,
                                btnLineDashed,
                                btnLineDotted,
                                1
                        );
                    }
                }
        );


        btnLineDotted.setOnClickListener(
                new View.OnClickListener() {

                    @Override
                    public void onClick(View v) {

                        Preferences.changeFeatureInt(
                                "Line Style",
                                21,
                                2
                        );

                        spinnerLineStyle.setSelection(
                                2
                        );

                        updateLineStyleButtons(
                                btnLineSolid,
                                btnLineDashed,
                                btnLineDotted,
                                2
                        );
                    }
                }
        );


        /*
         * LINE WIDTH
         * Backend: 1 até 10
         */

        int width =
                Preferences.loadPrefInt(
                        "Line Width",
                        22
                );

        if (width < 1) {
            width = 2;
        }

        width = clamp(width, 1, 10);

        seekLineWidth.setProgress(width - 1);

        txtLineWidthValue.setText(
                String.valueOf(width)
        );

        seekLineWidth.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {

                    @Override
                    public void onProgressChanged(
                            SeekBar seekBar,
                            int progress,
                            boolean fromUser
                    ) {

                        int value = progress + 1;

                        txtLineWidthValue.setText(
                                String.valueOf(value)
                        );

                        if (fromUser) {

                            Preferences.changeFeatureInt(
                                    "Line Width",
                                    22,
                                    value
                            );
                        }
                    }

                    @Override
                    public void onStartTrackingTouch(
                            SeekBar seekBar
                    ) {
                    }

                    @Override
                    public void onStopTrackingTouch(
                            SeekBar seekBar
                    ) {
                    }
                }
        );


        /*
         * LINE OPACITY
         */

        int opacity =
                clamp(
                        Preferences.loadPrefInt(
                                "Line Opacity",
                                23
                        ),
                        0,
                        100
                );

        seekLineOpacity.setProgress(opacity);

        txtLineOpacityValue.setText(
                opacity + "%"
        );

        seekLineOpacity.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {

                    @Override
                    public void onProgressChanged(
                            SeekBar seekBar,
                            int progress,
                            boolean fromUser
                    ) {

                        txtLineOpacityValue.setText(
                                progress + "%"
                        );

                        if (fromUser) {

                            Preferences.changeFeatureInt(
                                    "Line Opacity",
                                    23,
                                    progress
                            );
                        }
                    }

                    @Override
                    public void onStartTrackingTouch(
                            SeekBar seekBar
                    ) {
                    }

                    @Override
                    public void onStopTrackingTouch(
                            SeekBar seekBar
                    ) {
                    }
                }
        );


        /*
         * LANGUAGE
         *
         * 0 = English
         * 1 = Português
         * 2 = Español
         */

        String[] languages = {
                "English",
                "Português",
                "Español"
        };

        ArrayAdapter<String> languageAdapter =
                new ArrayAdapter<>(
                        context,
                        android.R.layout.simple_spinner_item,
                        languages
                );

        languageAdapter.setDropDownViewResource(
                android.R.layout.simple_spinner_dropdown_item
        );

        spinnerLanguage.setAdapter(languageAdapter);

        int language =
                clamp(
                        Preferences.loadPrefInt(
                                "Language",
                                28
                        ),
                        0,
                        2
                );

        spinnerLanguage.setSelection(language);

        spinnerLanguage.setOnItemSelectedListener(
                new AdapterView.OnItemSelectedListener() {

                    @Override
                    public void onItemSelected(
                            AdapterView<?> parent,
                            View selectedView,
                            int position,
                            long id
                    ) {

                        currentLanguage =
                                clamp(
                                        position,
                                        MenuTranslations.ENGLISH,
                                        MenuTranslations.SPANISH
                                );

                        Preferences.changeFeatureInt(
                                "Language",
                                28,
                                currentLanguage
                        );

                        /*
                         * Atualiza imediatamente:
                         * - header
                         * - footer
                         * - aba atual
                         */
                        applyTranslationsToView(
                                root,
                                currentLanguage
                        );
                    }

                    @Override
                    public void onNothingSelected(
                            AdapterView<?> parent
                    ) {
                    }
                }
        );
    }

    private void updateLineStyleButtons(
            Button solid,
            Button dashed,
            Button dotted,
            int selected
    ) {

        Button[] buttons = {
                solid,
                dashed,
                dotted
        };

        for (
                int index = 0;
                index < buttons.length;
                index++
        ) {

            Button button =
                    buttons[index];

            if (index == selected) {

                button.setBackgroundResource(
                        R.drawable.bg_sidebar_button_active
                );

                button.setTextColor(
                        COLOR_ACTIVE
                );

            } else {

                button.setBackgroundResource(
                        R.drawable.bg_pill_button
                );

                button.setTextColor(
                        COLOR_NORMAL
                );
            }
        }
    }


    /*
     * =========================================================
     * AUTO PLAY
     * =========================================================
     */

    private void showAutoPlay() {

        View view = inflateTab(
                R.layout.tab_autoplay,
                tabAutoPlay
        );

        enableVerticalScroll(
                view,
                R.id.scrollAutoPlay
        );

        final Switch swAutoPlay =
                view.findViewById(R.id.swAutoPlay);

        final Button btnSemiAuto =
                view.findViewById(R.id.btnSemiAuto);

        final Button btnFullAuto =
                view.findViewById(R.id.btnFullAuto);

        final SeekBar seekForce =
                view.findViewById(R.id.seekForce);

        final TextView txtForceValue =
                view.findViewById(R.id.txtForceValue);

        final SeekBar seekActionInterval =
                view.findViewById(R.id.seekActionInterval);

        final TextView txtIntervalValue =
                view.findViewById(R.id.txtIntervalValue);


        final Button btnStartSimulation =
                view.findViewById(
                        R.id.btnStartSimulation
                );

        final Button btnEndSimulation =
                view.findViewById(
                        R.id.btnEndSimulation
                );


        final TextView txtMatchState =
                view.findViewById(
                        R.id.txtMatchState
                );

        final TextView txtFsmState =
                view.findViewById(
                        R.id.txtFsmState
                );

        final TextView txtFsmCycle =
                view.findViewById(
                        R.id.txtFsmCycle
                );

        final TextView txtSimulatedScore =
                view.findViewById(
                        R.id.txtSimulatedScore
                );


        /*
         * ENABLE
         */

        boolean autoPlayEnabled =
                Preferences.loadPrefBool(
                        "Enable Auto Play",
                        0,
                        false
                );

        swAutoPlay.setChecked(autoPlayEnabled);

        swAutoPlay.setOnCheckedChangeListener(
                (buttonView, isChecked) ->
                        Preferences.changeFeatureBool(
                                "Enable Auto Play",
                                0,
                                isChecked
                        )
        );


        /*
         * MODE
         *
         * 0 = Disabled
         * 1 = Semi
         * 2 = Full
         */

        int mode =
                clamp(
                        Preferences.loadPrefInt(
                                "Mode",
                                1
                        ),
                        0,
                        2
                );

        updateAutoPlayModeButtons(
                btnSemiAuto,
                btnFullAuto,
                mode
        );

        btnSemiAuto.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {

                        Preferences.changeFeatureInt(
                                "Mode",
                                1,
                                1
                        );

                        updateAutoPlayModeButtons(
                                btnSemiAuto,
                                btnFullAuto,
                                1
                        );
                    }
                }
        );

        btnFullAuto.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {

                        Preferences.changeFeatureInt(
                                "Mode",
                                1,
                                2
                        );

                        updateAutoPlayModeButtons(
                                btnSemiAuto,
                                btnFullAuto,
                                2
                        );
                    }
                }
        );


        /*
         * FORCE
         */

        int force =
                clamp(
                        Preferences.loadPrefInt(
                                "Force",
                                2
                        ),
                        0,
                        100
                );

        seekForce.setProgress(force);

        txtForceValue.setText(
                force + "%"
        );

        seekForce.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {

                    @Override
                    public void onProgressChanged(
                            SeekBar seekBar,
                            int progress,
                            boolean fromUser
                    ) {

                        txtForceValue.setText(
                                progress + "%"
                        );

                        if (fromUser) {

                            Preferences.changeFeatureInt(
                                    "Force",
                                    2,
                                    progress
                            );
                        }
                    }

                    @Override
                    public void onStartTrackingTouch(
                            SeekBar seekBar
                    ) {
                    }

                    @Override
                    public void onStopTrackingTouch(
                            SeekBar seekBar
                    ) {
                    }
                }
        );


        /*
         * ACTION INTERVAL
         *
         * UI progress 0..2900
         * Backend 100..3000 ms
         */

        int interval =
                Preferences.loadPrefInt(
                        "Action Interval",
                        3
                );

        if (interval < 100) {
            interval = 1000;
        }

        interval = clamp(
                interval,
                100,
                3000
        );

        seekActionInterval.setProgress(
                interval - 100
        );

        txtIntervalValue.setText(
                interval + " ms"
        );

        seekActionInterval.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {

                    @Override
                    public void onProgressChanged(
                            SeekBar seekBar,
                            int progress,
                            boolean fromUser
                    ) {

                        int value =
                                progress + 100;

                        txtIntervalValue.setText(
                                value + " ms"
                        );

                        if (fromUser) {

                            Preferences.changeFeatureInt(
                                    "Action Interval",
                                    3,
                                    value
                            );
                        }
                    }

                    @Override
                    public void onStartTrackingTouch(
                            SeekBar seekBar
                    ) {
                    }

                    @Override
                    public void onStopTrackingTouch(
                            SeekBar seekBar
                    ) {
                    }
                }
        );

        /*
         * =====================================================
         * FSM SANDBOX SIMULATION
         * =====================================================
         *
         * Feature 7 = Simulate Match Start
         * Feature 9 = Simulate Match End
         */

        btnStartSimulation.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {

                        Preferences.changeFeatureInt(
                                "Simulate Match Start",
                                7,
                                0
                        );

                        android.widget.Toast.makeText(
                                context,
                                "Simulation started",
                                android.widget.Toast.LENGTH_SHORT
                        ).show();

                        btnStartSimulation.setText(
                                "Simulation running"
                        );
                    }
                }
        );

        btnEndSimulation.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {

                        Preferences.changeFeatureInt(
                                "Simulate Match End",
                                9,
                                0
                        );

                        android.widget.Toast.makeText(
                                context,
                                "Simulation ended",
                                android.widget.Toast.LENGTH_SHORT
                        ).show();

                        btnStartSimulation.setText(
                                MenuTranslations.get(
                                        currentLanguage,
                                        "start_simulation"
                                )
                        );
                    }
                }
        );
        startFsmStatusUpdates(
                view,
                txtMatchState,
                txtFsmState,
                txtFsmCycle,
                txtSimulatedScore
        );

    }

    private void startFsmStatusUpdates(
            final View tabView,
            final TextView txtMatchState,
            final TextView txtFsmState,
            final TextView txtFsmCycle,
            final TextView txtSimulatedScore
    ) {

        if (!(context instanceof FloatingModMenuService)) {
            return;
        }

        final FloatingModMenuService service =
                (FloatingModMenuService) context;

        final Runnable updater =
                new Runnable() {

                    @Override
                    public void run() {

                        if (!tabView.isAttachedToWindow()) {
                            return;
                        }

                        try {

                            String rawJson =
                                    service.getEngineStateJson();

                            org.json.JSONObject state =
                                    new org.json.JSONObject(
                                            rawJson
                                    );

                            String match =
                                    state.optString(
                                            "match",
                                            "IDLE"
                                    );

                            String fsm =
                                    state.optString(
                                            "fsm",
                                            "IDLE"
                                    );

                            long cycle =
                                    state.optLong(
                                            "cycle",
                                            0L
                                    );

                            int playerScore =
                                    state.optInt(
                                            "scorePlayer",
                                            0
                                    );

                            int opponentScore =
                                    state.optInt(
                                            "scoreOpponent",
                                            0
                                    );

                            txtMatchState.setText(
                                    match
                            );

                            txtFsmState.setText(
                                    fsm
                            );

                            txtFsmCycle.setText(
                                    String.valueOf(
                                            cycle
                                    )
                            );

                            txtSimulatedScore.setText(
                                    playerScore
                                            + " - "
                                            + opponentScore
                            );

                        } catch (Exception ignored) {

                            txtMatchState.setText(
                                    "ERROR"
                            );

                            txtFsmState.setText(
                                    "UNKNOWN"
                            );
                        }

                        tabView.postDelayed(
                                this,
                                150L
                        );
                    }
                };

        tabView.addOnAttachStateChangeListener(
                new View.OnAttachStateChangeListener() {

                    @Override
                    public void onViewAttachedToWindow(
                            View v
                    ) {
                    }

                    @Override
                    public void onViewDetachedFromWindow(
                            View v
                    ) {

                        tabView.removeCallbacks(
                                updater
                        );
                    }
                }
        );

        tabView.post(
                updater
        );
    }


    private void updateAutoPlayModeButtons(
            Button semi,
            Button full,
            int mode
    ) {

        semi.setBackgroundResource(
                mode == 1
                        ? R.drawable.bg_sidebar_button_active
                        : R.drawable.bg_pill_button
        );

        full.setBackgroundResource(
                mode == 2
                        ? R.drawable.bg_sidebar_button_active
                        : R.drawable.bg_pill_button
        );

        semi.setTextColor(
                mode == 1
                        ? COLOR_ACTIVE
                        : COLOR_NORMAL
        );

        full.setTextColor(
                mode == 2
                        ? COLOR_ACTIVE
                        : COLOR_NORMAL
        );
    }

    /*
     * =========================================================
     * AUTO QUEUE
     * =========================================================
     */

    private void showAutoQueue() {

        View view = inflateTab(
                R.layout.tab_autoqueue,
                tabAutoQueue
        );

        enableVerticalScroll(
                view,
                R.id.scrollAutoQueue
        );

        final Switch swAutoQueue =
                view.findViewById(R.id.swAutoQueue);

        final Spinner spinnerQueueMode =
                view.findViewById(R.id.spinnerQueueMode);

        final SeekBar seekCoins =
                view.findViewById(R.id.seekCoinsToUse);

        final TextView txtCoins =
                view.findViewById(R.id.txtCoinsValue);

        final Switch swHighTablesOnly =
                view.findViewById(R.id.swHighTablesOnly);

        final Switch swMixedJoin =
                view.findViewById(R.id.swMixedJoin);

        final Switch swFixSingleTable =
                view.findViewById(R.id.swFixSingleTable);


        /*
         * ENABLE
         */

        boolean enabled =
                Preferences.loadPrefBool(
                        "Enable Auto Queue",
                        4,
                        false
                );

        swAutoQueue.setChecked(enabled);

        swAutoQueue.setOnCheckedChangeListener(
                (buttonView, isChecked) ->
                        Preferences.changeFeatureBool(
                                "Enable Auto Queue",
                                4,
                                isChecked
                        )
        );


        /*
         * QUEUE MODE
         *
         * 0 Slow
         * 1 Global
         * 2 Fast
         */

        String[] queueModes = {
                MenuTranslations.get(
                        currentLanguage,
                        "slow"
                ),
                MenuTranslations.get(
                        currentLanguage,
                        "global"
                ),
                MenuTranslations.get(
                        currentLanguage,
                        "fast"
                )
        };

        ArrayAdapter<String> queueAdapter =
                new ArrayAdapter<>(
                        context,
                        android.R.layout.simple_spinner_item,
                        queueModes
                );

        queueAdapter.setDropDownViewResource(
                android.R.layout.simple_spinner_dropdown_item
        );

        spinnerQueueMode.setAdapter(queueAdapter);

        int queueMode =
                clamp(
                        Preferences.loadPrefInt(
                                "Queue Speed",
                                14
                        ),
                        0,
                        2
                );

        spinnerQueueMode.setSelection(
                queueMode
        );

        spinnerQueueMode.setOnItemSelectedListener(
                new AdapterView.OnItemSelectedListener() {

                    @Override
                    public void onItemSelected(
                            AdapterView<?> parent,
                            View selectedView,
                            int position,
                            long id
                    ) {

                        Preferences.changeFeatureInt(
                                "Queue Speed",
                                14,
                                position
                        );
                    }

                    @Override
                    public void onNothingSelected(
                            AdapterView<?> parent
                    ) {
                    }
                }
        );


        /*
         * COINS TO USE
         */

        int coins =
                clamp(
                        Preferences.loadPrefInt(
                                "Coins to Use",
                                15
                        ),
                        0,
                        100000
                );

        seekCoins.setProgress(coins);

        txtCoins.setText(
                String.valueOf(coins)
        );

        seekCoins.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {

                    @Override
                    public void onProgressChanged(
                            SeekBar seekBar,
                            int progress,
                            boolean fromUser
                    ) {

                        txtCoins.setText(
                                String.valueOf(progress)
                        );

                        if (fromUser) {

                            Preferences.changeFeatureInt(
                                    "Coins to Use",
                                    15,
                                    progress
                            );
                        }
                    }

                    @Override
                    public void onStartTrackingTouch(
                            SeekBar seekBar
                    ) {
                    }

                    @Override
                    public void onStopTrackingTouch(
                            SeekBar seekBar
                    ) {
                    }
                }
        );


        /*
         * HIGH TABLES
         *
         * Visual apenas por enquanto.
         * Ainda não possui feature ID próprio.
         */

        swHighTablesOnly.setChecked(false);
        swHighTablesOnly.setEnabled(false);
        swHighTablesOnly.setAlpha(0.45f);


        /*
         * MIXED JOIN
         */

        boolean mixed =
                Preferences.loadPrefBool(
                        "Mixed Join",
                        16,
                        false
                );

        swMixedJoin.setChecked(mixed);

        swMixedJoin.setOnCheckedChangeListener(
                (buttonView, isChecked) ->
                        Preferences.changeFeatureBool(
                                "Mixed Join",
                                16,
                                isChecked
                        )
        );


        /*
         * FIX SINGLE TABLE
         */

        boolean fixSingle =
                Preferences.loadPrefBool(
                        "Fix on Single Table",
                        17,
                        false
                );

        swFixSingleTable.setChecked(
                fixSingle
        );

        swFixSingleTable.setOnCheckedChangeListener(
                (buttonView, isChecked) ->
                        Preferences.changeFeatureBool(
                                "Fix on Single Table",
                                17,
                                isChecked
                        )
        );


        /*
         * TABLE SELECTION
         *
         * Feature 18 recebe o índice da mesa.
         */

        final int[] tableButtonIds = {
                R.id.btnTable25,
                R.id.btnTable100,
                R.id.btnTable500,
                R.id.btnTable1200,
                R.id.btnTable2500,
                R.id.btnTable3000,
                R.id.btnTable5000,
                R.id.btnTable10000,
                R.id.btnTable50500,
                R.id.btnTable101000,
                R.id.btnTable510000,
                R.id.btnTable2550000,
                R.id.btnTable4150000
        };

        int selectedTable =
                clamp(
                        Preferences.loadPrefInt(
                                "Table Selection",
                                18
                        ),
                        0,
                        tableButtonIds.length - 1
                );

        updateTableButtons(
                view,
                tableButtonIds,
                selectedTable
        );

        for (
                int index = 0;
                index < tableButtonIds.length;
                index++
        ) {

            final int selectedIndex = index;

            Button tableButton =
                    view.findViewById(
                            tableButtonIds[index]
                    );

            tableButton.setOnClickListener(
                    new View.OnClickListener() {

                        @Override
                        public void onClick(View v) {

                            Preferences.changeFeatureInt(
                                    "Table Selection",
                                    18,
                                    selectedIndex
                            );

                            updateTableButtons(
                                    view,
                                    tableButtonIds,
                                    selectedIndex
                            );
                        }
                    }
            );
        }
    }

    private void updateTableButtons(
            View view,
            int[] ids,
            int selectedIndex
    ) {

        for (
                int index = 0;
                index < ids.length;
                index++
        ) {

            Button button =
                    view.findViewById(
                            ids[index]
                    );

            if (index == selectedIndex) {

                button.setBackgroundResource(
                        R.drawable.bg_sidebar_button_active
                );

                button.setTextColor(
                        COLOR_ACTIVE
                );

            } else {

                button.setBackgroundResource(
                        R.drawable.bg_pill_button
                );

                button.setTextColor(
                        COLOR_NORMAL
                );
            }
        }
    }

    /*
     * =========================================================
     * TRANSLATION
     * =========================================================
     */

    private void applyTranslationsToView(
            View view,
            int language
    ) {

        if (view == null) {
            return;
        }

        /*
         * Button também herda de TextView,
         * então esse bloco traduz labels e botões.
         */
        if (view instanceof TextView) {

            TextView textView =
                    (TextView) view;

            CharSequence current =
                    textView.getText();

            if (current != null) {

                String key =
                        findTranslationKey(
                                current.toString()
                        );

                if (key != null) {

                    textView.setText(
                            MenuTranslations.get(
                                    language,
                                    key
                            )
                    );
                }
            }
        }

        /*
         * Percorre recursivamente o layout inteiro.
         */
        if (view instanceof android.view.ViewGroup) {

            android.view.ViewGroup group =
                    (android.view.ViewGroup) view;

            for (
                    int i = 0;
                    i < group.getChildCount();
                    i++
            ) {

                applyTranslationsToView(
                        group.getChildAt(i),
                        language
                );
            }
        }
    }


    private String findTranslationKey(
            String value
    ) {

        String[] keys = {

                "subtitle",
                "hide_kill",
                "minimize",

                "basic_title",
                "visual_options",
                "show_lines",
                "show_lines_desc",
                "keep_lines",
                "keep_lines_desc",

                "lines_title",
                "appearance",
                "style",
                "solid",
                "dashed",
                "dotted",
                "line_width",
                "line_opacity",
                "language",
                "language_desc",

                "autoplay_title",
                "simulation",
                "enable_autoplay",
                "enable_autoplay_desc",
                "autoplay_mode",
                "semi_auto",
                "full_auto",
                "force",
                "action_interval",

                "simulation_test",
                "simulation_test_desc",
                "start_simulation",
                "end_simulation",

                "autoqueue_title",
                "queue",
                "enable_autoqueue",
                "enable_autoqueue_desc",
                "queue_mode",
                "coins_to_use",
                "filtering",
                "high_tables",
                "mixed_join",
                "fix_single_table",
                "table_selection",

                "slow",
                "global",
                "fast"
        };

        for (String key : keys) {

            /*
             * Reconhece o texto independentemente
             * do idioma em que ele está atualmente.
             */

            if (
                    value.equals(
                            MenuTranslations.get(
                                    MenuTranslations.ENGLISH,
                                    key
                            )
                    )
                    ||
                    value.equals(
                            MenuTranslations.get(
                                    MenuTranslations.PORTUGUESE,
                                    key
                            )
                    )
                    ||
                    value.equals(
                            MenuTranslations.get(
                                    MenuTranslations.SPANISH,
                                    key
                            )
                    )
            ) {

                return key;
            }
        }

        return null;
    }


    /*
     * =========================================================
     * SCROLL
     * =========================================================
     */

    private void enableVerticalScroll(
            View tabView,
            int scrollId
    ) {

        final android.widget.ScrollView scrollView =
                tabView.findViewById(scrollId);

        if (scrollView == null) {
            return;
        }

        scrollView.setFillViewport(false);
        scrollView.setVerticalScrollBarEnabled(true);
        scrollView.setOverScrollMode(
                View.OVER_SCROLL_NEVER
        );

        if (android.os.Build.VERSION.SDK_INT >= 21) {
            scrollView.setNestedScrollingEnabled(true);
        }

        scrollView.setOnTouchListener(
                new View.OnTouchListener() {

                    @Override
                    public boolean onTouch(
                            View v,
                            android.view.MotionEvent event
                    ) {

                        /*
                         * Enquanto o dedo estiver dentro da aba,
                         * nenhum container externo deve assumir
                         * o gesto vertical.
                         */

                        if (event.getAction()
                                == android.view.MotionEvent.ACTION_DOWN
                                ||
                            event.getAction()
                                == android.view.MotionEvent.ACTION_MOVE) {

                            v.getParent()
                                    .requestDisallowInterceptTouchEvent(
                                            true
                                    );

                        } else if (
                                event.getAction()
                                        == android.view.MotionEvent.ACTION_UP
                                ||
                                event.getAction()
                                        == android.view.MotionEvent.ACTION_CANCEL
                        ) {

                            v.getParent()
                                    .requestDisallowInterceptTouchEvent(
                                            false
                                    );
                        }

                        /*
                         * false = o próprio ScrollView continua
                         * processando normalmente o movimento.
                         */

                        return false;
                    }
                }
        );
    }


    /*
     * =========================================================
     * UTIL
     * =========================================================
     */

    private int clamp(
            int value,
            int min,
            int max
    ) {

        if (value < min) {
            return min;
        }

        if (value > max) {
            return max;
        }

        return value;
    }
}
