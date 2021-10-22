# 中継器

```eval_rst
.. raw:: html

    <script>
        mermaid.initialize({
            theme: 'neutral',
        });
    </script>
```

中継器では入力部、出力部、15パズルの
依存関係を薄くするために各部分の橋渡し
を行う部分である。


以下では中継器の構成図と
そこで用いる関数の詳細について説明する。

## 入力部の構成図

```eval_rst
.. mermaid::

    graph TD;
        data(入力情報);
        style data fill:#43A047, stroke:#1B5E20;
        
        subgraph 中継器本体;
            recv["中継器"];
        end

        data --> recv
```

