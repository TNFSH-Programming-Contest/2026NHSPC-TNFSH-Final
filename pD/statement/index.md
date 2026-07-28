# Merge, Rebase, Squash!

\begin{figure}[h]
\centering
\begin{minipage}[t]{0.48\linewidth}
\centering
\includegraphics[width=\linewidth,height=2.2in,keepaspectratio]{db-force-push-1.jpg}\\
\textbf{Force push 1}
\end{minipage}
\hfill
\begin{minipage}[t]{0.48\linewidth}
\centering
\includegraphics[width=\linewidth,height=2.2in,keepaspectratio]{db-force-push-2.jpg}\\
\textbf{Force push 2}
\end{minipage}
\caption{不要學 DB force push (他現在已經不會了)}
\end{figure}

tobiichi3227 是 TOJ 的維護者，TOJ 整個程式碼由 $H$ 個獨立的 **hunk** 組成，每個 hunk 的狀態一開始都是 $0$，在 `v2.0` 分支上面。\
有天 wonderhoi 想給 TOJ 貢獻一個酷炫的功能，於是 wonderhoi 很高興的開啟了一個新的分支 `feat/fancy` 並在上面建立了 $N$ 個 commit。\
wonderhoi 想讓功能進到 TOJ 的 `v2.0`、，於是請 tobiichi3227 來 review，tobiichi3227 覺得 $N$ 個 commit 太多了，應該用 squash 把 commit 減少到只有 $K$ 個。\
當 squash 完成後，會將 `feat/fancy` rebase 到 `v2.0`，wonderhoi 發現會有 conflict，所以他想要最小化解決 conflict 的成本，但 wonderhoi 要去比地奧 (IESO) 沒空了，於是交給在看題目的你。

\clearpage

\begin{figure}[h]
\centering
\includegraphics[width=\linewidth]{git-workflow-storyboard.jpg}
\caption{Branch、squash、rebase 與 conflict resolution 的執行過程}
\end{figure}

若使用支援 PDF 附件的 Firefox，可\statementattachfile{git-workflow.gif}{點我下載完整動畫}；Chrome 與 Edge 請直接閱讀上方分鏡。

## Branch 狀態

`v2.0` branch 上第 $h$ 個 hunk 的最終內容為 $U_h$。\
`feat/fancy` branch 上共有 $N$ 個 commits。第 $i$ 個 commit 會將第 $x_i$ 個 hunk 的內容改成 $v_i$。\
令 $S_i[h]$ 表示執行完 `feat/fancy` branch 的前 $i$ 個 commits 後，第 $h$ 個 hunk 的內容。\
初始時：$S_0[h]=0$\
第 $i$ 個 commit 執行後：$S_i[x_i]=v_i$\
其他 hunk 的內容不變。

\clearpage

## Squash
原本的 $N$ 個 commits 依照順序排列：
```text
commit 1, commit 2, ..., commit N
```
你需要在這些 commits 之間放置 $K-1$ 個切點，將它們分成恰好 $K$ 組。\
每一組中的所有 commits 會被合併成一個新的 **squash commit**。\
例如，假設共有 $7$ 個 commits，且要分成 $3$ 組，可以選擇：
```text
[commit 1, commit 2]
[commit 3, commit 4, commit 5]
[commit 6, commit 7]
```
也可以簡寫成：
```text
[1, 2] [3, 4, 5] [6, 7]
```
這代表：
- 第一個 squash commit 包含原本的 commits $1$ 到 $2$
- 第二個 squash commit 包含原本的 commits $3$ 到 $5$
- 第三個 squash commit 包含原本的 commits $6$ 到 $7$

為了描述一般情況，令：$0=p_0<p_1<p_2<\cdots<p_K=N$\
其中 $p_g$ 表示第 $g$ 組的最後一個 commit 編號。\
因此，第 $g$ 個 squash commit 包含：$p_{g-1}+1,\ p_{g-1}+2,\ \ldots,\ p_g$\
例如前面的分組：
```text
[1, 2] [3, 4, 5] [6, 7]
```
對應：$p_0=0,\ p_1=2,\ p_2=5,\ p_3=7$\
考慮第 $g$ 個 squash commit，以及某個 hunk $h$。\
在這組 commits 開始以前，hunk $h$ 的內容是：$S_{p_{g-1}}[h]$\
執行完這組中的所有 commits 後，hunk $h$ 的內容是：$S_{p_g}[h]$\
因此，這個 squash commit 對 hunk $h$ 的效果為：$S_{p_{g-1}}[h]\rightarrow S_{p_g}[h]$\
也就是只保留這一組 commits 執行前與執行後的差異，中間經過的狀態不會出現在 squash commit 中。\
例如，某個 hunk 在一組 commits 中依序發生：
```text
1 -> 3 -> 5 -> 2
```
那麼 squash 後只會留下：
```text
1 -> 2
```
中間的 $3$ 與 $5$ 都不會出現在 squash commit 中。\
若一組 commits 執行前後，某個 hunk 的內容相同，即：$S_{p_{g-1}}[h]=S_{p_g}[h]$\
則這個 squash commit 不會修改該 hunk。\

\clearpage

## Rebase 與 Conflict
完成 squash 後，tobiichi3227 會依照原本的順序，將這 $K$ 個 squash commits 逐一套用到 `v2.0` branch 上。\
考慮其中一個 squash commit 對 hunk $h$ 的修改：$\text{old}\rightarrow\text{new}$\
其中：
- `old`：這個 squash commit 預期修改前的內容；
- `new`：這個 squash commit 想要修改成的內容；
- `current`：目前 `v2.0` branch 上實際存在的內容。

例如：$\text{old}=3,\ \text{new}=7,\ \text{current}=5$\
代表這個 squash commit 原本是在內容為 $3$ 的基礎上，將它改成 $7$，但目前 `v2.0` branch 上的內容實際是 $5$。

### 沒有修改
若：$\text{old}=\text{new}$\
代表這個 squash commit 執行前後，該 hunk 的內容沒有改變。\
因此不需要進行任何操作，`current` 也不會改變。


### 情況一：目前內容正是預期的舊內容
若：$\text{current}=\text{old}$\
代表目前內容與這個 patch 預期的起點相同，因此可以正常套用修改。\
套用後：$\text{current}\leftarrow\text{new}$\
例如：
```text
patch:   3 -> 7
current: 3
```
可以正常套用，完成後 $\text{current}$ 變成 $7$。\

### 情況二：目前內容已經是目標內容
若：$\text{current}=\text{new}$\
代表 `v2.0` branch 上已經存在這個 squash commit 想要產生的結果。\
因此不會發生 conflict，也不需要再次修改。\
例如：
```text
patch:   3 -> 7
current: 7
```
雖然目前內容不是 patch 預期的舊內容 $3$，但它已經等於目標內容 $7$，所以可以直接視為這項修改已經存在。\
$\text{current}$ 維持為 $7$。

### 情況三：目前內容既不是舊內容，也不是新內容
若：$\text{current}\ne\text{old}$\
且：$\text{current}\ne\text{new}$\
代表 `v2.0` branch 上的內容與這個 squash commit 預期的內容不同，而且也不是它想要修改成的結果。\
此時會發生 conflict，例如：
```text
patch:   3 -> 7
current: 5
```
目前內容 $5$ 既不是舊內容 $3$，也不是新內容 $7$，因此發生 conflict。\
解決 hunk $h$ 的 conflict 需要支付 $w_h$ 的成本。\
解決 conflict 後，採用 squash commit 的修改結果：$\text{current}\leftarrow\text{new}$\
在前面的例子中，解決後 $\text{current}$ 會變成 $7$。\
同一個 squash commit 可能同時修改多個 hunks。每個 hunk 是否發生 conflict 都要分別判斷，發生 conflict 的成本也要分別計算。

\clearpage

## 輸入
第一行包含三個整數 $N,\ H,\ K$，分別表示：
- `feat/fancy` branch 上的 commit 數量；
- hunk 的數量；
- squash 後必須保留的 commit 數量。

第二行包含 $H$ 個整數 $U_h\ (1\le h \le H)$\
其中 $U_h$ 表示 `v2.0` branch 上第 $h$ 個 hunk 的最終內容。\
第三行包含 $H$ 個整數 $w_h\ ( 1 \le h \le H)$\
其中 $w_h$ 表示解決第 $h$ 個 hunk 的 conflict 所需的成本。\

接下來 $N$ 行，第 $i\ (1 \le i \le N)$ 行包含兩個整數 $x_i$ 與 $v_i$\
表示第 $i$ 個 commit 將第 $x_i$ 個 hunk 的內容改成 $v_i$。\

## 輸出
輸出一個整數，表示最小的 conflict 解決總成本。

## 輸入限制
- $1\le N,\ H\le 200000$
- $1\le K\le \min(N,30)$
- $1\le U_h\le 10^9$
- $1\le w_h\le 10^9$
- $1\le x_i\le H$
- $1\le v_i\le 10^9$
- 對於每個 commit $i$：$v_i\ne S_{i-1}[x_i]$
- 保證每個 commit 都會實際改變該 hunk 的內容，且 `feat/fancy` branch 不會將任何 hunk 改回 $0$。

\clearpage

## 子任務
\subtasks

## 範例輸入
\testfile{0-01.in}

## 範例輸出
\testfile{0-01.out}

## 範例說明
`feat/fancy` branch 上兩個 hunk 的狀態依序如下：
```text
commit 0：  (0, 0)
commit 1：  (1, 0)
commit 2：  (1, 2)
commit 3：  (2, 2)
commit 4：  (2, 4)
commit 5：  (3, 4)
```
可以將 commits 分成：
```text
[1] [2, 3, 4, 5]
```
第一個 squash commit 將 hunk 1：
```text
0 -> 1
```
套用到 `v2.0` branch 時，hunk 1 的目前內容已經是 $1$，因此不會發生 conflict。\
第二個 squash commit 的效果為：\
```text
hunk 1：1 -> 3
hunk 2：0 -> 4
```
此時 hunk 1 的目前內容為 $1$，可以正常套用。\
hunk 2 的目前內容已經是 $4$，因此也不會發生 conflict。\
總成本為 $0$。

\clearpage

## 範例輸入
\testfile{0-02.in}

## 範例輸出
\testfile{0-02.out}

## 範例說明
因為 $K=1$，兩個 commits 必須全部 squash 在一起。\
Squash 後的效果為：\
```text
0 -> 2
```
但 `v2.0` branch 上該 hunk 的內容為 $1$。\
目前內容既不是 patch 的舊內容 $0$，也不是新內容 $2$，因此發生 conflict，成本為 $7$。

## 範例輸入
\testfile{0-03.in}

## 範例輸出
\testfile{0-03.out}

## 範例說明
因為 $K=2$，兩個 commits 分別套用。\
第一個 commit 的效果為：
```text
0 -> 1
```
而 `v2.0` branch 上目前的內容已經是 $1$，因此不會發生 conflict。\
第二個 commit 的效果為：
```text
1 -> 2
```
此時目前內容為 $1$，可以正常套用。\
因此總成本為 $0$。

## Note
<!-- TODO: Git 小教室 -->
