
================
モジュール構成図
================


.. mermaid::

  graph TD;
    ボタン入力-->入力部;
    キーボード入力-->入力部;
    入力部-->gameif;
    gameif[Midator];
    pazzlecore[15パズル]
    gameif-->pazzlecore;
    pazzlecore-->gameif;
    gameif-->出力部;
    出力部-->LEDマトリクス;
    出力部-->ターミナルテキスト;

-------
modules
-------

以下ではそれぞれのモジュールについて説明する。




