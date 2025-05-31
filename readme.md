# 사용자 가이드 - Hexdump Recreation

이 프로그램은 리눅스 `hexdump` 명령어를 개선한 C 기반 유틸리티입니다.  
출력 형식의 가독성을 높이고, 사용자 지정 옵션 및 파일 비교 기능을 제공합니다.

---

## 목차

1. [설치 방법](#1-설치-방법)
2. [사용법](#2-사용법)
3. [옵션 설명](#3-옵션-설명)
4. [출력 예시](#4-출력-예시)
5. [오류 및 주의사항](#5-오류-및-주의사항)
6. [이슈](#6-이슈-태그-설명)
7. [문의 및 라이선스](#7-문의-및-라이선스)

---

## 1. 설치 방법

이 프로젝트는 GCC 컴파일러를 사용하여 빌드할 수 있습니다.  
다음 단계를 따라 설치를 진행하세요:

1. 프로젝트를 클론합니다.

```bash
git clone https://github.com/Kcits970/hdim
```

2. 프로젝트 디렉토리로 이동합니다.

```bash
cd hdim
```

3. Makefile을 사용하여 컴파일합니다.

```bash
make
```

> ⚠️ `make` 명령 실행 시 `gcc`가 설치되어 있어야 합니다.  
> ⚠️ 컴파일이 완료되면 `./hdim` 실행 파일이 생성됩니다.

4. 프로그램이 정상적으로 빌드되었는지 확인합니다.

---

## 2. 사용법

기본 실행 형식:

```bash
./hdim [옵션] <파일이름>
```

예시:

```bash
./hdim -n 64 -C file.txt
```

파일 비교:

```bash
./hdim -D file1.txt file2.txt
```

---

## 3. 옵션 설명

| 옵션       | 설명                                                           |
|------------|----------------------------------------------------------------|
| `-V | --version` | 프로그램 버전 정보를 출력합니다. (`hdim (hexdump-improved) 1.0.0`) |
| `-b | --one-byte-octal` | 각 바이트를 8진수로 출력합니다. |
| `-c | --one-byte-char` | 각 바이트를 ASCII 문자 형태로 출력합니다. |
| `-C | --canonical` | 16진수와 그에 대응하는 ASCII 문자를 같이 출력합니다. |
| `-d | --two-bytes-decimal` | 2바이트 단위로 묶어서 10 진수로 출력합니다. |
| `-n | --length` | 파일의 첫 n바이트만 읽습니다. |
| `-o | --two-bytes-octal` | 2바이트 단위로 묶어서 8진수로 출력합니다. |
| `-s | --skip` | 지정된 오프셋만큼 건너뛰고 읽습니다. |
| `-x | --two-bytes-hex` | 2바이트 단위로 묶어서 16진수로 출력합니다. |
| `-M | --md5` | 주어진 파일의 MD5 체크섬을 출력합니다. |
| `-S | --sha256` | 주어진 파일의 SHA256 체크섬을 출력합니다. |
| `-F | --find` | 파일 내에서 주어진 문자열을 탐색합니다. |

---

## 4. 출력 예시



### 파일 비교 출력

```bash
./hdim -D hello.txt hello1.txt
```
![D](./example/D.png)
                                
### 파일 비교 출력(+다른 옵션)

```bash
./hdim -D -C hello.txt hello1.txt -n 16 
```
![D](./example/D_n.png)                  

```bash
./hdim -D -C hello.txt hello1.txt -n 16 -b
```
![D](./example/D_b.png) 

```bash
./hdim hello.txt -D hello1.txt -c -n 32
```
![D](./example/D_n.png) 





### 각 바이트 8진수 출력
```bash
./hdim -b test.txt
```
### 문자 출력
```bash
./hdim -c test.txt
```
### Canonical 출력
```bash
./hdim -C hello.txt
```
![C](./example/C.png)
### 2바이트 10진수 출력
```bash
./hdim -d test.txt
```

### 2바이트 8진수 출력
```bash
./hdim -o test.txt
```

### 버전 출력
```bash
./hdim -V
```
### MD5 Checksum 출력
``` bash
./hdim -M hello.txt
```
### 파일 해시값(Checksum) 출력
```bash
./hdim -S hello.txt
```
### 파일 내 키워드 검색
```bash
./hdim -F hello hello.txt
```






---

## 5. 오류 및 주의사항

- `-D` 옵션을 사용할 때 텍스트 파일 2개를 입력해야 합니다.
- `-D` 비교 모드에서 강조 색상은 두번째 파일에 적용됩니다.
- `-D` ASCII 영역 출력에 공백이 포함될 수 있습니다.
- 옵션은 순서와 무관하게 조합 가능합니다.



---

## 6. 이슈 (라벨 설명)
https://github.com/Kcits970/hdim/issues
Bug : 버그 제보
Docs : 문서 질문, 개선점 (ex:README.md)
Deploy : 배포 관련 문의
Feature : 기능 개선점 
Question : 추가적인 질문 

## 7. 문의 및 라이선스

GPL-3.0 license
문의: suyeanjong0323@dankook.ac.kr


