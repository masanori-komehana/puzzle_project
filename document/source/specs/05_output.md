# 出力部


```eval_rst
.. raw:: html

    <script>
        mermaid.initialize({
            theme: 'neutral',
        });
    </script>
```

ここでは出力部の構成図と
そこで用いる関数の詳細について説明する。

## 出力部の構成図

```eval_rst
.. mermaid::
    graph TD;

        subgraph sysTickHandler;
            LED[LEDマトリクス表示];
        end
        subgraph PC;
            key[端末画面出力];
        end

        subgraph 出力部本体;
            piece_data(駒情報);
            board_data(盤面情報);
            generate[画面情報生成];
            style data fill:#43A047, stroke:#1B5E20;
        end

        midator["中継器"];

        button --> decode;
        key --> decode;
        decode --> data;
        data --> midator
```
