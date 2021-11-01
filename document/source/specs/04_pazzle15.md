# パズル15本体
```eval_rst
.. raw:: html

    <script>
        mermaid.initialize({
            theme: 'neutral',
        });
    </script>
```

ここでは、パズルのロジック本体の実装を行う

## 関数構成図

```eval_rst
.. mermaid::


    graph TD;
        subgraph 中継器;
            mid_data(入力データ);
            cmd_pazzle[パズル操作] --> state_data;
            state_data(状態データ);
            mid_data --> cmd_pazzle;
            get_state[状態取得]-.-state_data;
            set_state[状態変更]-.-state_data;
        end

        subgraph 15パズル;
        move_direction(移動方向);
        board(盤面データ);

        pazzle_move[駒移動];
        get_blank_pos[空白位置取得];
        move_blank_pos[空白位置移動];
        board_init[盤面初期化];
        board_shuffle[盤面シャッフル];
        is_game_clear[クリア判定]-->set_state;
        end

        cmd_pazzle --方向入力--> move_direction --> pazzle_move --> get_blank_pos;
        cmd_pazzle --初期化入力--> board_init --> board_shuffle -.- board;
        board_init -.-> board;

        board_init -.-> get_state -->board_init;

        get_state ---> pazzle_move -.->get_state;

        board --> get_blank_pos --> move_blank_pos --> is_game_clear;

```
