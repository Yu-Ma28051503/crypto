# 色々な暗号を実装
## 暗号
### Vernam暗号
----------
Vernam暗号はAT&Tに勤めていたギルバード・バーナムが特許を取得した共通鍵暗号方式である．
平文と鍵のビット列で排他的論理和をとることで暗号化，暗語文と鍵を同じように排他的論理和をとることで復号化する方式である．

### Base32
----------
Base32は40ビットを8文字に変換するエンコード方式である．

### RSA
----------
RSA暗号は1977年にRivest, Shamir, Adlemanによって提案された公開鍵暗号方式である. 

素数$p$と$q$を用いて$n=pq$を計算し, オイラー関数$\phi(n)=(p-1)(q-1)$を計算する. 
次に, $\phi(n)$と互いに素な$e$を選び, $ed \equiv 1 \pmod{\phi(n)}$を満たす$d$を計算する. 
このとき, ($e$, $n$)を公開鍵, ($d$, $p$, $q$)を秘密鍵とする. 
暗号化は, 平文$m$を$0 \leq m < n$を満たす整数とし, $c \equiv m^e \pmod{n}$を計算する. 
復号は, $c^d \equiv (m^e)^d \equiv m \pmod{n}$を計算することで行う. 

### DSA
----------
DSAは有限体上の値と演算を使った署名アルゴリズムである.

**署名**

メッセージ$m = \{0, 1\}^*$ に対して, 
整数kをランダムで選び,
$r := (g^k \mod{q})$を計算して, 
署名sを, $s := (H(m) + xr) / k \mod{q}$ で生成する.
{$r$, $s$}をmに対する署名とする.

**検証**

署名{$r$, $s$} について,  
$ u_1 := H(m)s^{-1} \mod{q} $  
$ u_2 := rs^{-1} \mod{q} $  
を計算し,  
$ v := (g^{u_1}g^{u_2} \mod{p}) \mod{q} $  
を求めて $ v == r $ なら受理して, そうでないなら拒否する.

検証の正当性は,  
$ g^{u_1}g^{u_2} = g^{H(m)s^{-1}}(g^x)^{rs^{-1}} = g^{(H(m) + xr)s^{-1}} = g^{(H(m) + xr)(k/H(m) + xr)} = g^k = r $  
である. 
