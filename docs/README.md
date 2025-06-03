# hdim (hexdump-improved) 1.0.0

이 문서는 프로그램 개발자를 위한 가이드입니다.

---

## 목차

1. [프로젝트 구조](#1-프로젝트-구조)
2. [주요 파일 설명](#2-주요-파일-설명)
3. [코드 스타일 가이드](#3-코드-스타일-가이드)
4. [기능 추가시 규칙](#4-새로운-기능-추가-시-규칙)
5. [가이드라인](#5-기여-가이드라인)
6. [TODO](#6-todo)

---

## 1. 프로젝트 구조

```
hdim/
├── README.md
├── docs/
|   └── README.md
├── src/
│   ├── main.c
│   ├── args.c
│   ├── args.h
│   ├── dump.c
│   ├── dump.h
│   ├── find.c
│   ├── find.h
│   ├── checksum.c
│   ├── checksum.h
│   └── util.h
├── examples/
│   └── example text files, binary files, etc.
├── makefile
└── LICENSE
```

---

## 2. 주요 파일 설명

| 파일명 | 설명 |
|-|-|
| main.c | 진입점(main 함수) 정의 |
| args.c | hdim 옵션/인자 구조체 정의 포함, 옵션 파싱 기능 구현 |
| dump.c | 아스키/바이너리 출력 기능 구현 |
| find.c | 문자열 및 바이트 시퀀스 탐색 기능 구현 |
| checksum.c | 파일의 체크섬 (MD5, SHA256) 계산 기능 구현 |
---

## 3. 코드 스타일 가이드

- 항상 탭 문자(`\t`)를 이용하여 들여씁니다.
- 중괄호 배치는 Allman 스타일을 이용합니다.
```C
int function_name(int arg)
{
  // do something.
}
```
- 변수명/함수명은 특수한 경우를 제외하고 스네이크 표기법을 사용합니다. 상수는 대문자 스네이크 표기법을 사용합니다. 어떤 경우에도 이름에 소문자와 대문자를 섞지 않습니다.
```C
int another_function(int arg)
{
  const int MODE = 0x1234;
  int tmp = 1;
  int substr_len = 10;
  // int camelCaseIsNotAllowedHere;
}
```
- 함수 내부의 코드가 길어질 경우, 개행 문자를 적절히 이용하여 연관된 부분을 묶습니다.
```C
void long_function()
{
  // do something.

  // then do this.

  // then do that.

  // finally, clean-up.
}
```
- 코드 내부의 모든 주석은 영어로 작성합니다.
---

## 4. 새로운 기능 추가 시 규칙
- 모든 새로운 기능을 추가할 때는 적어도 하나 이상의 테스트 결과가 있어야 합니다.
---

## 5. 기여 가이드라인

- 커밋은 최대한 작은 단위로 해야 합니다.
- 커밋 메시지는 적어도 완전한 문장의 형태로 되어 있어야 합니다.

---

## 6. TODO
- [ ] `-D` 옵션에서 색상 설정을 CLI 인자에서 커스터마이징
- [ ] 2개 이상의 출력 옵션 혼합 허용

---
