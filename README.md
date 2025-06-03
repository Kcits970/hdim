# hdim (hexdump-improved) 1.0.0

이 프로그램은 GNU 유틸리티 `hexdump`를 개선한 C 기반 프로그램입니다.  
출력 형식의 가독성을 높이고, 바이트 탐색 및 파일 비교 기능을 제공합니다.

---

## 목차

1. [설치 방법](#1-설치-방법)
2. [사용법](#2-사용법)
3. [옵션 설명](#3-옵션-설명)
4. [출력 예시](#4-출력-예시)
5. [주의사항](#5-주의사항)

---

## 1. 설치 방법

> ⚠️ 프로그램을 빌드하려면 gcc 컴파일러가 꼭 있어야 합니다.

아래의 과정을 따라 프로그램을 빌드합니다.

1. 프로젝트를 클론합니다.

```bash
git clone git@github.com:Kcits970/hdim.git
```

2. 프로젝트 디렉토리로 이동합니다.

```bash
cd hdim
```

3. 주어진 makefile을 사용하여 컴파일합니다. make 기능이 없다면, gcc를 이용해도 됩니다.

```bash
make
```

```bash
gcc ./src/*.c -o hdim
```

4. `hdim` 실행 파일이 정상적으로 생성되었는지 확인합니다.

```bash
ls -l | grep hdim
```

---

## 2. 사용법

프로그램이 받는 인자의 완전한 형식은 아래와 같습니다.

```bash
./hdim [-V] [-F|-F8|-F16 <sequence>] [-M|-S] <filename> [-b|-c|-C|-d|-o|-x] [-n <length>] [-s <skip>]
```

가장 단순한 형식은 아래와 같습니다.
```bash
./hdim <filename>
```

---

## 3. 옵션 설명

| 옵션 | 풀 옵션 | 설명 |
|-|-|-|
| -V | --version | 프로그램 버전 정보를 출력합니다. (hdim (hexdump-improved) 1.0.0) |
| -M | --md5 | 주어진 파일의 MD5 체크섬을 출력합니다. |
| -S | --sha256 | 주어진 파일의 SHA256 체크섬을 출력합니다. |
| -F | --find | 파일 내에서 주어진 문자열을 탐색합니다. |
| -F8 | --find-octal | 파일 내에서 주어진 바이트 시퀀스를 탐색합니다. (8진수) |
| -F16 | --find-hex | 파일 내에서 주어진 바이트 시퀀스를 탐색합니다. (16진수) |
| -b | --one-byte-octal | 각 바이트를 8진수로 출력합니다. |
| -c | --one-byte-char | 각 바이트를 ASCII 문자 형태로 출력합니다. |
| -C | --canonical | 16진수와 그에 대응하는 ASCII 문자를 같이 출력합니다. |
| -d | --two-bytes-decimal | 2바이트 단위로 묶어서 10 진수로 출력합니다. |
| -n | --length | 파일의 첫 n바이트만 읽습니다. |
| -o | --two-bytes-octal | 2바이트 단위로 묶어서 8진수로 출력합니다. |
| -s | --skip | 지정된 오프셋만큼 건너뛰고 읽습니다. |
| -x | --two-bytes-hex | 2바이트 단위로 묶어서 16진수로 출력합니다. |

---

## 4. 출력 예시
간단한 예시를 위하여 examples 디텍토리에 몇 가지 예제 파일을 넣어놓았습니다. 

### 기본 옵션들만 사용
```bash
./hdim examples/hello
```
![image](https://github.com/user-attachments/assets/429ef622-683e-4b1c-8b1c-81d08bd704ab)

### -C (--canonical) 옵션
```bash
./hdim -C examples/hello
```
![image](https://github.com/user-attachments/assets/a864e29a-acca-42c4-bf0f-c148ff71bd1f)

### -o (--two-bytes-octal) 옵션
```bash
./hdim -o examples/hello
```
![image](https://github.com/user-attachments/assets/af6e973f-b4be-49f7-b14a-72a20f4d4425)

### -c (--one-byte-char) 옵션
```bash
./hdim -c examples/hello
```
![image](https://github.com/user-attachments/assets/62186a97-454b-4093-baba-e3fc2341ac27)

### -s (--skip), -n (--length) 옵션
```bash
./hdim -C examples/hello
./hdim -C examples/hello -s 6 -n 16
```
![image](https://github.com/user-attachments/assets/efd0e535-2760-4231-afda-4d0fd3600cb0)

### -M (--md5), -S (--sha256) 옵션
```
./hdim -M examples/hello
./hdim -S examples/hello
```
![image](https://github.com/user-attachments/assets/3c50fa21-cccd-4b00-b482-ecce26dc98d0)

### -F (--find), -F16 (--find-hex) 옵션
```bash
./hdim examples/red -F IHDR
./hdim examples/red -F16 "49 48 44 52"
```
![image](https://github.com/user-attachments/assets/0d9f3142-891c-42f1-9398-bdbcc1b25539)

### -D (--difference) 옵션
```bash
./hdim -C examples/red -D examples/blue
```
![image](https://github.com/user-attachments/assets/80920a8e-bc7d-49da-aed6-30dc0598d08e)

---

## 5. 주의사항

같이 조합하여 쓸 수 있는 옵션이 있고, 그렇지 못한 옵션도 있습니다.

- `-V` 옵션을 사용하면, 나머지 모든 옵션들은 무시됩니다.
- 출력 형식 지정 옵션은 둘 이상을 조합하여 사용할 수 없습니다. 따라서 `-b`, `-c`, `-C`, `-d`, `-o`, `-x` 옵션은 한번에 하나만 설정할 수 있습니다.
- 체크섬 옵션도 마찬가지로 둘 이상을 조합할 수 없습니다. `-M`, `-S` 중 한번에 하나 이하만 사용할 수 있습니다.
- 체크섬 옵션이 설정되어 있으면, 해당 파일과 체크섬 이외의 옵션들은 무시됩니다.
- 찾기 옵션도 한번에 한가지만 사용 가능합니다. `-F`, `F8`, `-F16` 옵션을 둘 이상 같이 사용할 수 없습니다.
- 찾기 옵션이 있을 경우, 찾기 옵션과 무관한 옵션들은 전부 무시됩니다.

---
