# 你可以幫我吹口哨嗎?

\begin{figure}[h]
\centering
\includegraphics[width=5in]{photo.jpg}
\caption{Can You Blow My Whistle Baby}
\end{figure}

$\color{red}\text{本題是互動題型。}$

國中營比賽當天，中午吃飯的時候，有 $n$ 個小學弟排成一列準備領便當。

小學弟們身上穿著 $1$ 到 $n$ 號的衣服，且每個號碼恰好出現一次。

目前他們站在位置 $1$ 到 $n$ 上，位於位置 $i$ 的學弟是 $a_i$。

身為大學長的你，希望他們能依照號碼 $1$ 到 $n$ 的順序從小到大排好（也就是位置 $i$ 要站的是編號 $i$ 的學弟）。

為了節省時間，你決定吹哨子發號施令。每次吹哨，你可以同時指定任意多組學弟互換位置，但為了避免碰撞，每個學弟在同一次吹哨中，最多只能參與一組交換。

身為口哨先輩的你，請安排交換方式，用盡可能少的哨音讓所有學弟排好。

\clearpage

## 實作細節

你需要完成以下函式:

- `void solve(int n, std::vector<int> a);`
    - `a` 的長度為 $n$，且是 $1$ 到 $n$ 的一個排列。
    - 對於 $0 \le i < n$，`a[i]` 表示一開始站在位置 $i+1$ 的學弟編號。

上述程序的執行過程你需要呼叫以下函式:

- `void swap_student(int u, int v);`
    - $1 \le u, v \le n$ 且 $u \neq v$。
    - 將「交換位置 $u$ 與位置 $v$」加入目前這一輪，但此時還不會立刻交換。
    - 同一輪中，每個位置最多只能出現在一次 `swap_student` 呼叫中。
- `void blow_whistle();`
    - 同時執行目前這一輪加入的所有交換，接著開始新的一輪。
    - 即使這一輪沒有加入任何交換，呼叫此函式仍會計入一次哨音。

傳入的 `a` 是普通的 `std::vector<int>`；呼叫上述函式不會自動修改你程式中的 `a`。\
若 `solve` 結束時仍有尚未經過 `blow_whistle` 執行的交換，這些交換不會生效。

## 條件限制

- $1 \le n \le 10^5$
- $1 \le a_i \le n$
- $a$ 是一個 $1 \sim n$ 的排列

## 子任務

\subtasks

\clearpage

## 給分方式

假如你呼叫 `blow_whistle` 函式的次數\textbf{超過了 $2n$ 次}，則系統會中斷，並給出 \textbf{Wrong Answer}。

當你的 `solve` 函式結束時，系統會檢查學弟們是否已經正確排序。若最終沒有完成排序，該筆測資的得分倍率為 $0$。

若成功完成排序，令該筆測資理論上最少需要的哨音數為 $opt$，你呼叫 `blow_whistle` 的總次數為 $A$，該筆測資的得分倍率 $T$ 為：

- 若 $A=opt$，則 $T=1$。
- 若 $A>opt$，則 $T=\max(0,\ 0.5-0.02(A-opt))$。

也就是說，只要哨音數不是最佳解，該筆測資的倍率最多為 $0.5$，且每多吹一次會再扣除 $0.02$，直到降為 $0$。

對於滿分為 $P$ 的子任務，令 $T_{\min}$ 為你在該子任務所有測資中得到的最小倍率，則此子任務的得分為 $P \times T_{\min}$。

## 範例

考慮以下呼叫。

```cpp
solve(5, {2, 1, 3, 5, 4})
```

你可以在 `solve` 函式中依序呼叫：

```cpp
swap_student(1, 2);
swap_student(4, 5);
blow_whistle();
```

前兩個呼叫將兩組交換加入同一輪；呼叫 `blow_whistle` 後，兩組交換同時執行，排列變成 $[1,2,3,4,5]$。此例最少需要一次哨音。

\clearpage

## 範例評分程式

輸入格式 :

\noindent\fbox{%
\begin{minipage}{\dimexpr\textwidth-2\fboxsep-2\fboxrule\relax}
\raggedright
$
\begin{array}{l}
    n \\
    a_0, a_1, \ldots, a_{n-1}
\end{array}
$
\end{minipage}%
}

若呼叫函式過程中不違法，則輸出格式如下:

\noindent\fbox{%
\begin{minipage}{\dimexpr\textwidth-2\fboxsep-2\fboxrule\relax}
\raggedright
$
\begin{array}{l}
    A : \text{吹哨次數} \\
    a'_0, a'_1, \ldots, a'_{n-1}
\end{array}
$
\end{minipage}%
}

否則，格式為:

\noindent\fbox{%
\begin{minipage}{\dimexpr\textwidth-2\fboxsep-2\fboxrule\relax}
\raggedright
$
\begin{array}{l}
    \text{Wrong Answer: MSG}
\end{array}
$
\end{minipage}%
}

其中 MSG 是錯誤的原因，包含以下結果:

 - `invalid swap` : 你呼叫 `swap_student` 的 `u, v` 是不合法的。
 - `twice swap` : 有一個位置在同一輪交換中被 `swap_student` 函式呼叫到兩次以上了。

範例評分程式會在發現以上錯誤時回報錯誤並立即結束程式。除了以上有提到的錯誤之外，範例評分程式\textbf{不會}另外檢查答案的正確性。

正式的評分程式不一定採用以上程式輸入。請不要自行處理輸入輸出。
