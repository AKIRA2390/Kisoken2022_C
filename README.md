# Kisoken2022_C

# これは何?
基礎研C班の制御関係のファイルをだいたいまとめてるリポジトリ  開発に使ってたやつを公開用にちょっといじったもの。テスト勉強の現実逃避のため、比較的充実したreadmeがある。
esp32(M5Stamp pico)を使ってPCとWi-Fiでつなげて、PCでは和田先生のスクリプトを走らせて、それに対応するようにSocket通信で指示を送り付けるマイコンのプログラムと、その制作過程でできたあれこれ。  
日本語苦手…

gitで管理する必要がないようなのも含まれているけれど、後輩のためになったりするかもしれないしならないかもしれないし、みたいな  

## Special Thanks: 和田先生
[和田先生のスクリプト](https://github.com/TakeshiWada1980/Dobot_M1_Control)がなければこの方法は多分思いつきもしなかった気がするので、和田先生にはとても感謝しています。この場を借りてお礼申し上げます。

# What's inside

## Blockly
PoCとして、UIFlowを使って和田先生のスクリプトと通信させてみるテストをした。UIFlowはBlocklyというブロックプログラミング環境を使用してM5Stack製品のプログラムを作成できるサービスなのだけれど、UIFlowはネイティブではソケット通信をサポートしないので、カスタムブロック機能を使用してソケット通信のブロックを作成した。  
+ TCP_Blockly  
そのカスタムブロックのバージョン1で、送信のみ可能
+ TCP_Blockly_recv  
バージョン2で、帰ってきたステータスを確認するくらいの受信もできる。たぶんちゃんと公開したらそれなりの人の役に立つはず？

## kisokenC  
実物のコード。もうちょっと綺麗に書けるはずだけど、時間が足りなくて動かすこと優先のため汚い。結局受信処理は書かなかった。無しでもいけると思ってたので。過去形。
vscode + platformioで書いてる。コンパイルも書き込みも結構時間がかかる～、どうにかしたいけど時間がなかった。

## M5stick
M5StickC Plusのプログラムが入ってる。Blocklyのやつ。なぜM5StickC Plusなのかというと、ひとつには机の上にあったからで、もう一つにはfeature-richだから。バッテリーと液晶が載ってて、それをすごく楽に操作できるライブラリがあるので、こういうパッと試してみるのに向いてると思うのです。
+ Dobot_BareTest.m5f   
和田先生のスクリプトのコマンド形式でパケットを投げつけるやつ。TCP_Blockly_recvに依存してるので、それを先にインポートしないと読めない。
+ TCP_Test.m5f  
PoCに実際に使用したやつ。TCP_Blocklyに依存してる。
## TestScripts
和田先生のスクリプトを、PoCに使えるように、コマンド形式が違う～っていうエラーを出す機能を無効化したやつ。たぶんもう使わない。
+ ConnectionChecker.py  
ほとんど和田先生のスクリプトそのまま。権利的にはすごくあやしい。

## Dobot_KisokenC.script
ロボットアームのクライアントソフトであるM1 studioで動くプログラム。ただのpythonなのに拡張子を変えてて気に食わない。せめて.m1scriptとかならまだ許せたかもしれない。

## ipad_connect.bat
実は、上述のあれこれはipadのテザリングによってもたらされるwifiネットワーク上で通信をしていた。ネットワークわかんないのでナンセンスなのかもしれない。学校のlanを使おうとするとプロキシがいるしip空間なんか変な感じだしで、こっちのほうが楽な気がしたから。ネットワークわかんないのでナンセンスなのかもしれない。
それで、ipadに接続してipを固定するのをスクリプトにしようと思い、できたのがこちら。wifiのパスワードが入ってるけど、高専祭直後に変えたので無問題。

## playback.playback
M1 studioでアームを動かす時の座標メモとして使ってたファイル。拡張子が気に食わない。

# 付録: 基礎研とは？
高専祭に展示してたやつ。面倒なのでルール説明は省略する、外からも見えるルール説明とかが見つけられたら追加するかも。