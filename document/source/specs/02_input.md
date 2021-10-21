# 入力部

## 関数構成図

```eval_rst
.. mermaid::

    graph TD;
        
        subgraph sysTickHandler;
            button[ボタン入力];
        end
        subgraph PC
            key[キーボード入力];
        end
        
        subgraph 入力部本体;
            decode[入力処理];
            data>ボタンデータ];
            style data fill:#43A047, stroke:#1B5E20;
        end

        midator["中継器"];

        button --> decode;
        key --> decode;
        decode --> data;
        data --> midator
```

