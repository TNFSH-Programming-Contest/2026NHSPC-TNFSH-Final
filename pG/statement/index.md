# Performance Analysis

<!-- \begin{figure}[h]
\centering
\includegraphics[width=2in]{TODO.jpg}
\caption{TODO: 圖片說明}
\end{figure}
CFG
-->

tobiichi3227 跟 yushiuan9499 最近在開發 NCC，tobiichi3227 發現它程式執行太慢了，所以想給 NCC 加上一個功能，統計每一條 control-flow edge 會執行幾次。\
編譯器將程式用控制流圖 (Control Flow Graph，後面簡稱 CFG) 表示，CFG 上面會有 $N$ 個基本塊 (Basic Block，後面簡稱 BB)，每個 BB 會有幾個有向邊連到其他 BB，代表從 $BB_i$ 可以走到 $BB_j$，總共有 $M$ 條有向邊。\
程式會從入口 $1$ 開始，最後從出口 $N$ 離開，總共執行 $R$ 次程式。\
對於每條邊，編譯器在它所連接的 BB 上插入性能計數器 ($BB_i$ 或 $BB_j$ 都可以，編譯器只會選一個 BB 插入)，因為測量執行次數會造成額外的運算，所以插入每個計數器需要成本 $c_e$，執行結束後就能知道該邊通過的次數。\
對於沒有裝計數器的邊，可以用 flow conservation 推導計算得知：

- 對於普通節點: $\displaystyle \sum_{e \in in(v)}{f_e}=\sum_{e \in out(v)}{f_e}\ (v \neq 1,\ N)$
- 對於入口: $\displaystyle \sum_{e \in out(1)}{f_e}- \sum_{e \in in(v)}{f_e}=R$
- 對於出口: $\displaystyle \sum_{e \in in(N)}{f_e}- \sum_{e \in out(N)}{f_e}=R$

求最小安裝成本，使得執行結束後能夠**唯一確定每一條邊通過的次數**。

\clearpage

## 輸入
第一行會有三個整數 $N,\ M,\ R$，代表 BB 的數量、邊的數量與執行次數\
接下來 $M$ 行會有三個整數 $u\ v\ C$，代表 $BB_u$ 可以走到 $BB_v$，且安裝計數器成本為 $C$

## 輸出
最小安裝成本

## 輸入限制
- $1 \le N \le 10^5$
- $1 \le M \le 2 \times 10^5$
- $2M \le R \le 10^{18}$
- $1 \le u, v \le N$
- $1 \le C_e \le 10^9$
- 允許 self-loop 與重邊
- 保證所有 BB 一定有一個 $1$ 到 $N$ 的路徑通過

## 子任務
\subtasks

\clearpage

## 範例輸入
\testfile{0-01.in}

## 範例輸出
\testfile{0-01.out}

## 範例解釋

這張 CFG 的結構如下：

<!-- TODO: image for CFG -->
```text
      2
     / \
1 --    -- 4
     \ /
      3
```

程式總共執行 $8$ 次。每次執行會選擇下列其中一條路線：\
$1 \to 2 \to 4$ 或 $1 \to 3 \to 4$ \
可以花費 $2$，在邊 $1 \to 3$ 上安裝計數器。\
假設程式執行完畢後，計數器顯示邊 $1 \to 3$ 總共被通過 $3$ 次。因為程式總共執行 $8$ 次，所以邊 $1 \to 2$ 必定被通過 $8-3=5$ 次。\
每次進入 $BB_2$ 後都會沿著邊 $2 \to 4$ 離開，因此邊 $2 \to 4$ 也被通過 $5$ 次。\
同理，邊 $3 \to 4$ 被通過 $3$ 次。\
因此，只要在邊 $1 \to 3$ 上安裝一個計數器，就能確定所有邊的通過次數。\
若不安裝任何計數器，則無法判斷 $8$ 次執行中分別有多少次經過 $BB_2$ 或 $BB_3$。因此至少需要安裝一個計數器。\
所有邊中最低的安裝成本為 $2$，所以最小總費用為 $2$。

## NOTE:
<!-- TODO: 編譯器小教室 -->
