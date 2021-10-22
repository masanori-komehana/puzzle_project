# 入力部

```eval_rst
.. raw:: html

    <script>
        mermaid.initialize({
            theme: 'neutral',
        });
    </script>
```

ここでは入力部の構成図と
そこで用いる関数の詳細について説明する。

## 入力部の構成図

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
            data(入力情報);
            style data fill:#43A047, stroke:#1B5E20;
        end

        midator["中継器"];

        button --> decode;
        key --> decode;
        decode --> data;
        data --> midator
```

