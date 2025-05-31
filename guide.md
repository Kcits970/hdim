# 개발자 가이드 - Hexdump Recreation

이 문서는 프로젝트에 기여하거나, 유지보수 및 내부 구조를 이해하고자 하는 개발자를 위한 문서입니다.

---

## 목차

1. [프로젝트 구조](#1-프로젝트-구조)
2. [빌드 방법](#2-빌드-방법)
3. [주요 파일 설명](#3-주요-파일-설명)
4. [코드 스타일 가이드](#4-코드-스타일-가이드)
5. [기능 추가시 규칙](#5-새로운-기능-추가-시-규칙)
6. [가이드라인](#6-기여-가이드라인)
7. [TODO](#7-todo)
8. [이슈 및 PR 라벨](#8-이슈-라벨)
9. [문의 및 라이선스](#9-문의-및-라이선스)
---

## 1. 프로젝트 구조

```
hdim/
├── src/
│   ├── main.c
│   ├── args.c
│   ├── args.h
│   ├── dump.c
│   ├── dump.h
│   ├── checksum.c
│   ├── checksum.h
│   ├── byte_sequence.c
│   ├── byte_sequence.h
│   └── util.h
├── makefile
└── LICENSE
```

---

## 2. 빌드 방법

```bash
make
```

> 컴파일 시 GCC가 필요하며, `./hdim` 실행 파일이 생성됩니다.

---

## 3. 주요 파일 설명

| 파일명          | 설명 |
|------------------|------|
| `main.c`         | 진입점(main 함수) 정의 |
| `dump.c`         | hdim 기본 출력 기능 구현 |
| `checksum.c`     | 파일의 MD5, SHA256 계산 |
| `byte_sequence.c`| 파일내 문자열 탐색 기능 |
| `args.c`         | hdim 옵션/인자 구조체 정의 포함, 옵션 파싱 |
---

## 4. 코드 스타일 가이드

- 들여쓰기: 8칸 탭 문자
- 중괄호 스타일: K&R, Allman
- 함수명: 소문자 + 언더바 (`snake_case`)

---

## 5. 새로운 기능 추가 시 규칙
- 모든 새로운 기능은 `src/` 내부에 파일을 추가
- 기능 추가 관련 이슈는 Feature 라벨 사용

---

## 6. 기여 가이드라인

1. `develop` 브랜치에서 작업해주세요.
2. 기능 추가 전 이슈를 등록하거나 코멘트로 알려주세요.
3. PR 올릴 때 적합한 라벨을 사용하세요. ( EX: 버그 수정 -> BugFix )
3. 커밋 메시지는 명확하게 써주세요. 예:

```
[feature] add color logic to -D option
[fix] handle null byte in ascii output
```

---

## 7. TODO
- [ ] `-D` 옵션에서 색상 설정을 CLI 인자에서 커스터마이징

---

## 8. 이슈 라벨
https://github.com/Kcits970/hdim/issues
BugFix : 버그 수정
Docs : 문서 개선점 (ex:README.md)
Deploy : 배포 관련 개선
setting : 개발 환경 세팅 문의
Feature : 기능 개선 및 새로운 기능 개발
Question : 추가적인 질문 

---

## 9. 문의 및 라이선스

GPL-3.0 license
문의: suyeanjong0323@dankook.ac.kr

