#include "skiplist_impl.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <mutex>

// DefaultSkipList 생성자
DefaultSkipList::DefaultSkipList(int max_level, float prob)
{
    this->max_level_ = max_level;
    this->prob_ = prob;

    header_ = new Node();
    header_->key = -1;
    header_->value = -1;
    header_->upd_cnt = 0;
    header_->level = max_level_;

    header_->forward = new Node *[max_level_ + 1];
    for (int i = 0; i <= max_level_; i++)
    {
        header_->forward[i] = nullptr;
    }

    srand(time(nullptr));
}

// DefaultSkipList 소멸자
DefaultSkipList::~DefaultSkipList()
{
    Node *current = header_;
    Node *temp;

    while (current)
    {
        temp = current->forward[0];
        delete[] current->forward;
        delete current;
        current = temp;
    }
}

// 랜덤 레벨 생성 함수
int DefaultSkipList::random_level()
{
    int level = 0;
    while (((double)rand() / RAND_MAX) < prob_ &&
           level < max_level_)
    {
        level++;
    }
    return level;
}

// SkipList 생성자
SkipList::SkipList(int max_level, float prob) : DefaultSkipList(max_level, prob) {}

// SkipList 소멸자
SkipList::~SkipList() {}

// ==== SkipList: insert (단일 스레드, 락 없음) ====
void SkipList::insert(int key, int value)
{
    // update[i]: 레벨 i에서 삽입 위치 바로 직전 노드를 저장할 배열
    Node *update[max_level_ + 1];
    // current: 탐색용 포인터, 헤더부터 시작
    Node *current = header_;

    // 1) 최상위 레벨부터 레벨 0까지 삽입 위치 탐색
    for (int i = current_level_; i >= 0; --i)
    {
        // forward[i]가 존재하고 그 키가 삽입할 키보다 작으면 계속 전진
        while (current->forward[i] && current->forward[i]->key < key)
            current = current->forward[i];
        // 삽입 위치 직전 노드 기록
        update[i] = current;
    }

    // 2) 레벨 0에서 실제 위치 확인
    current = update[0]->forward[0];
    if (current && current->key == key)
    {
        // 이미 같은 키가 존재하면 value를 누적하고 upd_cnt 증가
        current->value += value;
        current->upd_cnt += 1;
        return;
    }

    // 3) 새로운 노드의 레벨 결정 (무작위)
    int lvl = random_level();
    if (lvl > current_level_)
    {
        // 레벨이 확장되면 새 레벨의 update도 헤더로 초기화
        for (int i = current_level_ + 1; i <= lvl; ++i)
            update[i] = header_;
        // 최고 레벨 갱신
        current_level_ = lvl;
    }

    // 4) 새 노드 생성 및 필드 초기화
    Node *new_node = new Node();
    new_node->key = key;                     // 키 저장
    new_node->value = value;                 // 값 저장
    new_node->upd_cnt = 0;                   // 업데이트 횟수 초기화
    new_node->level = lvl;                   // 레벨 설정
    new_node->forward = new Node *[lvl + 1]; // forward 배열 할당

    // 5) forward 포인터 연결 (splice)
    for (int i = 0; i <= lvl; ++i)
    {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
}

// ==== SkipList: lookup (단일 스레드, 락 없음) ====
int SkipList::lookup(int key)
{
    // current: 탐색용 포인터, 헤더부터 시작
    Node *current = header_;

    // 최상위 레벨부터 레벨 0까지 내림차순 탐색
    for (int i = current_level_; i >= 0; --i)
    {
        // forward[i]가 존재하고 그 키가 찾는 키보다 작으면 전진
        while (current->forward[i] && current->forward[i]->key < key)
            current = current->forward[i];
    }
    // 레벨 0 다음 노드로 이동
    current = current->forward[0];
    // 키가 일치하면 value, 아니면 0 반환
    return (current && current->key == key) ? current->value : 0;
}

// ==== SkipList: remove (단일 스레드, 락 없음) ====
void SkipList::remove(int key)
{
    Node *update[max_level_ + 1]; // 삭제 위치 직전 노드 저장
    Node *current = header_;      // 탐색용 포인터

    // 1) 최상위 레벨부터 레벨 0까지 삭제 위치 탐색
    for (int i = current_level_; i >= 0; --i)
    {
        while (current->forward[i] && current->forward[i]->key < key)
            current = current->forward[i];
        update[i] = current;
    }

    // 2) 삭제 대상 노드
    current = update[0]->forward[0];
    if (current && current->key == key)
    {
        // 3) forward 포인터 재연결 (unlink)
        for (int i = 0; i <= current_level_; ++i)
        {
            if (update[i]->forward[i] != current)
                break;
            update[i]->forward[i] = current->forward[i];
        }
        // 4) 메모리 해제
        delete[] current->forward;
        delete current;
        // 5) 최고 레벨 조정: 더 이상 노드가 없으면 레벨 감소
        while (current_level_ > 0 && header_->forward[current_level_] == nullptr)
            --current_level_;
    }
}

// CoarseSkipList 생성자
CoarseSkipList::CoarseSkipList(int max_level, float prob) : DefaultSkipList(max_level, prob)
{
    pthread_mutex_init(&mutex_lock, nullptr);
}

// CoarseSkipList 소멸자
CoarseSkipList::~CoarseSkipList()
{
    pthread_mutex_destroy(&mutex_lock);
}

// ==== CoarseSkipList: insert ====
void CoarseSkipList::insert(int key, int value)
{
    // 뮤텍스 획득: 전체 리스트를 단일 락으로 보호 시작
    pthread_mutex_lock(&mutex_lock);

    // update[i]: 레벨 i에서 삽입 직전 노드를 저장할 배열 선언
    Node *update[max_level_ + 1];
    // current: 탐색용 포인터, 헤더부터 시작
    Node *current = header_;

    // 1) 최상위 레벨부터 레벨 0까지 삽입 위치 탐색
    for (int i = current_level_; i >= 0; --i)
    {
        // forward[i]가 존재하고 그 키가 삽입할 키보다 작으면 전진
        while (current->forward[i] && current->forward[i]->key < key)
            current = current->forward[i];
        // 해당 레벨의 삽입 직전 노드 기록
        update[i] = current;
    }
    // 레벨 0 다음 노드로 이동하여 실제 위치 확인
    current = update[0]->forward[0];

    // 2) 중복 키인 경우: 값 누적 및 upd_cnt 증가
    if (current && current->key == key)
    {
        current->value += value; // 값 누적
        current->upd_cnt += 1;   // 업데이트 횟수 증가
    }
    else
    {
        // 3) 새 노드 레벨 결정 (무작위)
        int lvl = random_level();
        // 결정된 레벨이 기존 최고 레벨보다 크면
        if (lvl > current_level_)
        {
            // 확장된 레벨의 update도 헤더로 초기화
            for (int i = current_level_ + 1; i <= lvl; ++i)
                update[i] = header_;
            // 최고 레벨 갱신
            current_level_ = lvl;
        }
        // 4) 새 노드 동적 할당 및 필드 초기화
        Node *new_node = new Node();
        new_node->key = key;
        new_node->value = value;
        new_node->upd_cnt = 0;
        new_node->level = lvl;
        new_node->forward = new Node *[lvl + 1]; // forward 배열 할당

        // 5) forward 포인터 연결 (splice)
        for (int i = 0; i <= lvl; ++i)
        {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
    }

    // 뮤텍스 해제: 전체 삽입 과정 보호 종료
    pthread_mutex_unlock(&mutex_lock);
}

// ==== CoarseSkipList: lookup ====
int CoarseSkipList::lookup(int key)
{
    // 전체 탐색을 뮤텍스로 보호 시작
    pthread_mutex_lock(&mutex_lock);

    // current: 탐색용 포인터, 헤더부터 시작
    Node *current = header_;
    // 최상위 레벨부터 레벨 0까지 순차 탐색
    for (int i = current_level_; i >= 0; --i)
    {
        // forward[i]가 존재하고 그 키가 찾는 키보다 작으면 전진
        while (current->forward[i] && current->forward[i]->key < key)
            current = current->forward[i];
    }
    // 레벨 0 다음 노드로 이동
    current = current->forward[0];
    // 결과 값 결정: 키가 일치하면 value, 아니면 0
    int result = (current && current->key == key) ? current->value : 0;

    // 뮤텍스 해제: 탐색 보호 종료
    pthread_mutex_unlock(&mutex_lock);
    return result;
}

// ==== CoarseSkipList: remove ====
void CoarseSkipList::remove(int key)
{
    // 전체 삭제 과정을 뮤텍스로 보호 시작
    pthread_mutex_lock(&mutex_lock);

    // update[i]: 레벨 i에서 삭제 직전 노드를 저장할 배열 선언
    Node *update[max_level_ + 1];
    // current: 탐색용 포인터, 헤더부터 시작
    Node *current = header_;

    // 1) 최상위 레벨부터 레벨 0까지 삭제 위치 탐색
    for (int i = current_level_; i >= 0; --i)
    {
        // forward[i]가 존재하고 그 키가 삭제할 키보다 작으면 전진
        while (current->forward[i] && current->forward[i]->key < key)
            current = current->forward[i];
        // 해당 레벨의 삭제 직전 노드 기록
        update[i] = current;
    }
    // 레벨 0 다음 노드로 이동: 삭제 대상 노드
    current = update[0]->forward[0];

    // 2) 삭제 대상 노드가 존재하고 키가 일치할 때만 수행
    if (current && current->key == key)
    {
        // 3) forward 포인터 재연결 (unlink)
        for (int i = 0; i <= current_level_; ++i)
        {
            if (update[i]->forward[i] != current)
                break; // 더 이상 레벨에 존재하지 않으면 중단
            update[i]->forward[i] = current->forward[i];
        }
        // 4) 메모리 해제
        delete[] current->forward;
        delete current;
        // 5) 최고 레벨 축소: 노드가 없으면 레벨 감소
        while (current_level_ > 0 && header_->forward[current_level_] == nullptr)
            --current_level_;
    }

    // 뮤텍스 해제: 전체 삭제 보호 종료
    pthread_mutex_unlock(&mutex_lock);
}

// FineSkipList 생성자
FineSkipList::FineSkipList(int max_level, float prob) : DefaultSkipList(max_level, prob)
{
    current_level_ = 0;
    // 기본 헤더 삭제 후 FineNode로 교체
    delete header_;
    header_ = new FineNode();

    // 헤더 노드 락 초기화
    pthread_mutex_init(&((FineNode *)header_)->lock, nullptr);

    // 헤더의 기본값 설정
    header_->key = -1;
    header_->value = -1;
    header_->upd_cnt = 0;
    header_->level = max_level_;

    // forward 포인터 배열 초기화
    header_->forward = new Node *[max_level_ + 1];
    for (int i = 0; i <= max_level_; i++)
    {
        header_->forward[i] = nullptr;
    }
    pthread_mutex_init(&level_mutex_, nullptr); // ← 추가
}

// FineSkipList 소멸자
FineSkipList::~FineSkipList()
{
    FineNode *current = (FineNode *)header_;
    FineNode *temp;

    while (current)
    {
        temp = (FineNode *)current->forward[0];
        pthread_mutex_destroy(&((FineNode *)current)->lock);
        delete[] current->forward;
        delete current;
        current = temp;
    }

    header_ = nullptr;
    pthread_mutex_destroy(&level_mutex_); // ← 추가
}

// ==== FineSkipList: insert ====
// – Optimistic validation
// – forward 배열은 topLevel+1 크기로 최소 할당
// – std::sort + std::unique 사용으로 정렬 간소화
void FineSkipList::insert(int key, int value)
{
    int topLevel = random_level();

    // stack-array로 preds/succs 저장
    FineNode *preds[max_level_ + 1];
    FineNode *succs[max_level_ + 1];

    while (true)
    {
        // 1) lock-free 탐색: 각 레벨별 pred/succ 채우기
        FineNode *p = static_cast<FineNode *>(header_);
        for (int lvl = current_level_; lvl >= 0; --lvl)
        {
            FineNode *c = static_cast<FineNode *>(p->forward[lvl]);
            while (c && c->key < key)
            {
                p = c;
                c = static_cast<FineNode *>(p->forward[lvl]);
            }
            preds[lvl] = p;
            succs[lvl] = c;
        }
        // 확장된 레벨만 header/nullptr로 초기화
        for (int lvl = current_level_ + 1; lvl <= topLevel; ++lvl)
        {
            preds[lvl] = static_cast<FineNode *>(header_);
            succs[lvl] = nullptr;
        }

        // 2) toLock 수집
        FineNode *toLockArr[(max_level_ + 1) * 2];
        int lockCnt = 0;
        for (int i = 0; i <= topLevel; ++i)
        {
            toLockArr[lockCnt++] = preds[i];
            if (succs[i])
                toLockArr[lockCnt++] = succs[i];
        }
        // 주소 오름차순 정렬 + 중복 제거
        std::sort(toLockArr, toLockArr + lockCnt);
        lockCnt = std::unique(toLockArr, toLockArr + lockCnt) - toLockArr;

        // 3) lock
        for (int i = 0; i < lockCnt; ++i)
            pthread_mutex_lock(&toLockArr[i]->lock);
//            toLockArr[i]->lock_node();

        // 4) validation: preds→forward가 succs와 여전한지 확인
        bool valid = true;
        for (int i = 0; i <= topLevel; ++i)
        {
            if (preds[i]->forward[i] != succs[i])
            {
                valid = false;
                break;
            }
        }
        if (!valid)
        {
            for (int i = 0; i < lockCnt; ++i)
                pthread_mutex_unlock(&toLockArr[i]->lock);
//                toLockArr[i]->unlock_node();
            continue; // 재시도
        }

        // 5) current_level_ 갱신 (insert 시에만 증가)
        if (topLevel > current_level_)
            current_level_ = topLevel;

        // 6) 중복 키 처리 또는 splice
        if (succs[0] && succs[0]->key == key)
        {
            succs[0]->value += value; // 값 누적
            succs[0]->upd_cnt += 1;   // 업데이트 횟수 증가
        }
        else
        {
            // forward 배열은 topLevel+1 크기로만 할당
            FineNode *node = new FineNode();
            node->key = key;
            node->value = value;
            node->upd_cnt = 0;
            node->level = topLevel;
            node->forward = new Node *[topLevel + 1];
            // 필요한 레벨만 splice
            for (int i = 0; i <= topLevel; ++i)
            {
                node->forward[i] = preds[i]->forward[i];
                preds[i]->forward[i] = node;
            }
        }

        // 7) unlock
        for (int i = 0; i < lockCnt; ++i)
            pthread_mutex_unlock(&toLockArr[i]->lock);
//            toLockArr[i]->unlock_node();
        return;
    }
}

// ==== FineSkipList: lookup ====
// lock-free 탐색 후, target 노드만 잠금하여 값 읽기
int FineSkipList::lookup(int key)
{
    // 1) lock-free로 레벨별 탐색
    FineNode *p = static_cast<FineNode *>(header_);
    for (int lvl = current_level_; lvl >= 0; --lvl)
    {
        FineNode *c = static_cast<FineNode *>(p->forward[lvl]);
        while (c && c->key < key)
        {
            p = c;
            c = static_cast<FineNode *>(p->forward[lvl]);
        }
    }
    // 레벨 0 다음 노드가 target
    FineNode *t = static_cast<FineNode *>(p->forward[0]);
    if (!t || t->key != key)
        return 0;

    // 2) target 노드만 락 → 값 읽기 → 언락
//    t->lock_node();
    pthread_mutex_lock(&t->lock);
    int v = t->value;
    pthread_mutex_unlock(&t->lock);
//    t->unlock_node();
    return v;
}


// ==== FineSkipList: remove ====
// Optimistic validation remove: preds+succs 모두 잠금 → validate → unlink → unlock
void FineSkipList::remove(int key)
{
    FineNode *preds[max_level_ + 1];
    FineNode *succs[max_level_ + 1];

    while (true)
    {
        // 1) lock-free로 preds/succs 수집
        FineNode *pred = static_cast<FineNode *>(header_);
        for (int lvl = current_level_; lvl >= 0; --lvl)
        {
            FineNode *curr = static_cast<FineNode *>(pred->forward[lvl]);
            while (curr && curr->key < key)
            {
                pred = curr;
                curr = static_cast<FineNode *>(pred->forward[lvl]);
            }
            preds[lvl] = pred;
            succs[lvl] = curr;
        }

        // 2) 삭제 대상 노드 확인
        FineNode *target = succs[0];
        if (!target || target->key != key)
            return; // 없으면 바로 종료
        int topLevel = target->level;

        // 3) 잠글 노드 목록 수집: preds[0..topLevel] + target
        std::vector<FineNode *> toLock;
        toLock.reserve(topLevel + 2);
        for (int i = 0; i <= topLevel; ++i)
            toLock.push_back(preds[i]);
        toLock.push_back(target);
        std::sort(toLock.begin(), toLock.end());
        toLock.erase(std::unique(toLock.begin(), toLock.end()), toLock.end());

        // 4) 주소 오름차순으로 mutex lock
        for (auto n : toLock)
            pthread_mutex_lock(&n->lock);
//            n->lock_node();

        // 5) validation: preds[i]->forward[i]이 여전히 succs[i]인지 확인
        bool valid = true;
        for (int i = 0; i <= topLevel; ++i)
        {
            if (preds[i]->forward[i] != succs[i])
            {
                valid = false;
                break;
            }
        }
        if (!valid)
        {
            // 검증 실패 시 잠금 해제 후 재시도
            for (auto n : toLock)
                pthread_mutex_unlock(&n->lock);
//                n->unlock_node();
            continue;
        }

        // 6) unlink: preds[i]->forward[i] = target->forward[i]
        for (int i = 0; i <= topLevel; ++i)
            preds[i]->forward[i] = target->forward[i];

        // 7) mutex unlock
        for (auto n : toLock)
            pthread_mutex_unlock(&n->lock);
//            n->unlock_node();

        // 8) 물리적 삭제 생략 (unlink-only)
        return;
    }
} 