![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/bfa0443d-199f-41f0-9cba-d3bf398392b1/Untitled.png)

## 소개
객체 설계와 알고리즘 구현을 목표로 해당 게임을 모작하기로 결정하여 만든 이 게임은 2.5D의 로그라이크 장르의 액션 슈팅 인디 게임이다.

## 기능
- 다양한 디자인 패턴을 활용한 클래스 공용화, 유닛 관리, UI 종속성 및 중복성 개선
- 시스템 구축 프로세스를 통한 시스템 화면 변경, 이미지 to 비디오 재생, Scene 관리자, 스마트 포인터 구현
- 성능 개선과 사용성 강화를 위해 길 찾기 A*, Object Pooling, MapTool의 타일 크기 확장 구현

## 프로그램 개발
<details>
<summary>구현 기능</summary>
<div markdown="1">

### 길 찾기 알고리즘 A*
![ASTAR.gif](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/02a03c84-1be4-47a3-a4a6-619a65a09f1d/ASTAR.gif)

Enemy가 Player에게 다가가기 위해 오직 목적지만 탐색하는 A* 적합하다 판단, A* 설계 중 목적지(player 위치)까지 계산 방식인 F = G(시작점 기준 현재 노드까지 비용) + H(현재 노드에서 목적지까지 거리)를 ParentNode(최소 비용으로 계산된 이전 노드들)에 담아 이를 enemy에게 이동 명령 처리와 이동 중 원거리 특성상 유효 사거리에 들어올 경우 이동 중단 명령 처리가 필요하다 판단, 이를 매 프레임마다 enemy 거리(시작점)와 player 거리(도착점)을 구해 a* 수행, 단, 적 유효 사거리일 경우 중단 조건과 매 프레임마다 a* 수행은 성능 이슈로 이어질 것으로 판단하여 player가 이동하지 않는다면 a*는 한 번만 수행하게끔 최적화 개선

### **타일 크기 확장 기능 구현**
![제목 없는 동영상 - Clipchamp로 제작.gif](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/730dab34-3815-4618-8429-5573e43c19ea/%EC%A0%9C%EB%AA%A9_%EC%97%86%EB%8A%94_%EB%8F%99%EC%98%81%EC%83%81_-_Clipchamp%EB%A1%9C_%EC%A0%9C%EC%9E%91.gif)

맵툴 알고리즘 개발 과정에서 타일 설치 시 임의로 크기를 확장해야 하는 필요성 인식, 구현 시 타일을 드래그하여 설치 시 크기를 어떻게 확장시킬지에 대한 해결책이 필요. 드래그 이후 충돌 시점에서 새 좌표를 통해 타일 크기를 확장하는 방법 도입하여 문제 해결

### 오브젝트 풀링 활용 오버헤드 개선
![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/aa72580f-93c0-477b-ae21-6cf76acffd23/Untitled.png)

초당 소모되는 총알 생성 및 소멸 시 오버헤드 우려, 이는 게임 성능 저하와 게임 몰입 방해로 이어질 것으로 판단, 이는 초당 100여개 이상 즉 100ms 이상 비용 발생,  60프레임 렌더링에서 100ms / 1/60 * 100% = 166%이라는 치명적 성능 저하. 따라서 오브젝트 풀링 기술 도입. 200여개 객체를 미리 생성 후 풀에 저장 이를 풀에서 재활용 처리하며 관리. 도입 결과 166% 성능 저하 최소화 성과 달성

### 추상팩토리 디자인 패턴(유닛 관리)
![추상팩토리 생산.gif](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/59836e5c-41ab-404b-bbbf-4649933039b6/%EC%B6%94%EC%83%81%ED%8C%A9%ED%86%A0%EB%A6%AC_%EC%83%9D%EC%82%B0.gif)

팩토리패턴은 타입 별로 생산할 수 없어, Monster를 구현한 객체들을 타입 별로 생산할 수 있는 추상 팩토리 패턴이 적합 판단, 해당 클래스에 각 타입을 반환 메서드들을 기입하여 상황에 맞게 생산할 수 있게 처리 → ex 제품 추상 팩토리의 메서드 A는 오직 Monster를 상속한 A 객체만 생산

### 싱글톤 디자인패턴
여러 씬에서 메모리 공간에 얽매이지 않고 공용으로 사용할 유틸 클래스들을 어떻게 사용해야 할까 고민, 이를 하나의 전역 메모리에서 사용할 수 있는 싱글톤 디자인패턴을 고려, 하지만 각 유틸마다 타입이 다르기 때문에 하나의 메모리에서 사용하게 할 수 있을지 고민. template(제네릭)할 경우 공용화가 가능할 수 있다 판단 및 시도 결과 여러 씬에서 Sound, Screen, Option 등 여러 클래스를 공용 사용할 수 있게 해결

### Image Video 인트로 구현(비디오 재생 알고리즘)
![오프닝을 재생시키기 위해 여러 장의 그림들을 하나의 이미지에 압축.](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/8c06f7a3-d14d-4434-99cf-3f1c267a6dfa/Untitled.png)

오프닝을 재생시키기 위해 여러 장의 그림들을 하나의 이미지에 압축.

![제목 없는 동영상 - Clipchamp로 제작.gif](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/0b7098de-e75e-4354-a522-d1623e632f77/%EC%A0%9C%EB%AA%A9_%EC%97%86%EB%8A%94_%EB%8F%99%EC%98%81%EC%83%81_-_Clipchamp%EB%A1%9C_%EC%A0%9C%EC%9E%91.gif)

winAPI 버전 호환 문제로 video api 사용 불가능, 대안으로 영상을 이미지로 변환하여 출력 방법을 고려, 하지만 수백장 이미지를 어떻게 묶어 재생할 수 있을지 고민, 이를 해결하기 위해 기존 image 클래스에 재생 옵션을 추가한 video 클래스와 제어하기 위한 video controller를 도입하여 프레임 단위로 영상 재생하도록 구현

### GameSceneManager(gameSceneMgr) 성능 개선
![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/05ec5627-45c9-444f-8e22-87d192b29fe0/Untitled.png)

gameSceneMgr에서 관리되는 Scene 외 요소들 낭비되고 있는 것 발견, 이로 인해 게임 Scene의 로딩과 동작 중 성능 저하가 발생할 것으로 판단. CPU Scheduling Algorithm 도입 과정에서 ready, blocked, running 구조화를 접목 후 최적화 시도 결과, 재사용성 가능과 실행 중 씬 외의 자원 낭비 방지, 10% 성능 향상

### 상태 패턴을 활용한 여러 씬에서 UI 버튼 클래스 재사용성 향상
![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/bfc1e4a5-0922-438a-be2e-5bd33e9158db/Untitled.png)

여러 씬들 효율적 관리 고민, 각 씬에서 버튼 클래스를 개별 사용함으로써 코드 중복성 증가 및 유지보수가 떨어짐. 이를 각 상태에 대응할 수 있는 상태 디자인 패턴이 적합 판단 후 접목 후 버튼 관련  코드 최소화 달성, 이를 통해 유지보수성과 생산성 향상

### 해상도 변경 알고리즘
![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/da7a67e3-436c-4176-b93f-7d997576288a/Untitled.png)

![제목 없는 동영상 - Clipchamp로 제작 (1) (1).gif](https://prod-files-secure.s3.us-west-2.amazonaws.com/65262fac-94c7-45be-9937-5dd1fbde6f5a/859fedaf-354d-4d51-b7dc-f9d4db85cae1/%EC%A0%9C%EB%AA%A9_%EC%97%86%EB%8A%94_%EB%8F%99%EC%98%81%EC%83%81_-_Clipchamp%EB%A1%9C_%EC%A0%9C%EC%9E%91_(1)_(1).gif)

화면 변경 시 픽셀, 비율, 스케일 조정에 대한 필요성 고려, 픽셀에 따른 비율 고려와 조정, 스케일 계산에 대한 문제 발생, 이를 화면에 따른 비율 레퍼런스를 참고하여 계산 공식을 연구, (가로 픽셀 / 가로 비율) * 세로 비율의 계산 결과 토대로 GCD(최대공약수) 알고리즘 사용을 하면 되는 것을 알게됨. 이를 통해 계산, 스케일 계산은 게임 수학 행렬과 스케일 이론을 참고 해상도 * 스케일 비율 계산 사용

### 스마트포인터
Unmanaged 언어 특성상 프로그램 규모가 커짐에 따라 메모리 누수 가능성 고려, 이를 해결하기 위해 스마트포인터 도입, 각 객체 has-a 관계인 포인터 객체가 의존성이 높을 경우 unique_ptr을 사용, 역이라면 shared_ptr를 사용하여 상황을 고려하여 설계

</div>
</details>

<details>
<summary>회고</summary>
<div markdown="1">

### 새로운 출발
수료 이후 부족했던 문법과 문제해결능력을 개선하고 개인 프로젝트를 시작했을 때, 하나의 작품을 스스로 만들 수 있다는 자신감과 개발자란 긍지를 갖고 있었고, 환경 구축부터 공용적인 객체 설계까진 무리없이 진행했다. 

### 과정
하지만 진행 중 다양한 어려움에 부딪히게 되었다. 객체 설계의 한계, 예상치 못한 변수들로 인한 복잡한 문제들. 마치 여러 개의 무거운 추들을 몸에 달고 다니는 듯한 기분이었다. 어찌됐든 앞으로 나아가야만 했고, 가장 먼저 객체 설계부터 개선하기로 결정했다. 하나의 모듈에서 여러 개 역할을 다 수행하다 보니 유지보수가 힘들고, 오류를 찾고 해결하기가 매우 버거웠다. 그래서 관련 서적을 통해 개념을 익히며 리팩토링을 진행하여 새 시스템을 구축하여 해결하였다. 그리고 기능 구현에 대한 이슈들, OS와 상호협력으로 문제를 해결해 나아가야 할 부분들이 대다수였기 때문에, 원론적 기술 분석과 이해 그리고 추론 능력과 응용력을 요구하였다. 아직까지도 기억에 남는 더블버퍼링(화면 출력 기술) 이슈. 분명히 bmp 이미지를 백버퍼에 전달 후 이를 프론트 버퍼에 복사하여 화면에 출력 명령을 내렸는데 그림이 계속 밀리는 현상이 발생하는 문제에 대해 기술 원리를 다시 분석하고 두 버퍼 간의 전달 과정에서 백버퍼와 프론트버퍼의 전달 이후 데이터를 청소하는 것을 빠뜨렸던 점은 아직도 생생하게 기억한다. 그리고 A* 길찾기 알고리즘을 몬스터 객체에 주입하여 플레이어가 목표가 되어 따라가게 하는 요구조건은 실시간으로 움직이는 목표 좌표에 대한 상대 좌표 계산이 필요해서, 상당히 애를 먹기도 했었다.

### 이것만큼은
무언가를 체득하기 위해선 반드시 어렵게 해결해 나가는 과정이 필요하다고 생각한다. 이러한 과정을 통해 객체지향적 사고와 문제 해결 능력에 필요한 분석, 응용, 인내력을 키울 수 있었다. 게임전용 엔진같은 도구를 사용해서 생산성을 높일 수도 있었지만, 뿌리치고 끝까지 개발한 점에 대해서 아직도 정말 잘했다고 생각이 든다.

### 앞으로
첫 언어는 C언어였다. 그래서 뭐 하나를 하더라도 깊게 분석하고 잘못된 점을 개선해 나아가야만 했었고, 이것이 곧 자부심으로 까지 이어졌다. 하지만 이제는 더 다양한 기술을 습득하고 빠른 시간 내에 효율적인 결과를 도출하는 개발자가 되고 싶다. 직업을 시작하게된 계기인 필요한 제품을 만들어내는 창작자로서의 길을 걷고 싶을 뿐이다.
</div>
</details>

<details>
<summary>유의할 점</summary>
<div markdown="1">
  
![227455629-3c952306-5110-4606-9cac-364a09f79ac5](https://user-images.githubusercontent.com/63230518/227720467-91c547af-376a-44fe-a037-3340859dcbcf.png)

이전 커밋 기록 .git 파일 (git Lfs 이슈로 인해 커밋 초기화할 수 밖에 없어 남겨둡니다.)

https://drive.google.com/file/d/1PjhTHGqkLZwO74MkHWBBOHL-_e-vhfge/view?usp=sharing

#### 해당 프로젝트에 넣어야 할 대용량 이미지 파일
![227455619-acb7fa37-25f6-4c93-b5cd-6ead33b6a0e7](https://user-images.githubusercontent.com/63230518/227720465-03f656ad-53cf-438d-83f0-70adcaadf921.png)
https://drive.google.com/file/d/1N_XjV-e7wwBK-BdkCtuTEju2pL8QZwac/view?usp=sharing

사진 속 가리키는 폴더 내부에 붙여 넣으시면 됩니다.
</div>
</details>



