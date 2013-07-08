MIN_CREATION_DISTANCE = 100 -- 현재 위치로부터 장애물의 생성거리
MAX_CREATION_DISTANCE = 120 -- 현재 위치로부터 장애물의 생성거리
PLAYERWIDTH = 25 -- 플레이어의 가로 길이 (클 수록 잘 부딪힘)
MAX_SPEED = 5 -- 플레이어 최대 속도
MIN_SPEED = 3
DEFAULT_SPEED = 4 -- 초기 기본 속도
COMBO_RESET_TIME = 3 -- 이 시간동안 콤보를 하지 않으면 패널티가 주어짐
FLAG_MIN_CREATION_DISTANCE = 600 -- 현재 위치로부터 깃발의 최소 생성거리
FLAG_MAX_CREATION_DISTANCE = 700 -- 현재 위치로부터 깃발의 최대 생성거리
CHA_CRITERION = 50 -- 장애물 촤 판정 기준. 작을 수록 촤 하기 힘듬.
STUN_INIT_TIME = 0.7   	-- 맞았을 때 스턴 되는 시간.
UNBEATABLE_INIT_TIME = 1 -- 무적 타임
FRONT_DURATION = 0.8 -- 직진 시 제일 앞까지 가는데 걸리는 시간
BACK_DURATION = 1.2 -- 방향을 바꿀 시 뒤로 가는데 걸리는 시간
ZERO_DEC_VALUE = -0.05 -- 제로모드시 계속 깎이는 게이지의 정도
INC_VALUE_IN_ZERO = 10 -- 제로모드에서 콤보를 할 경우 채워지는 게이지의 양
INC_VALUE_IN_NORMAL = 20 -- 노멀모드에서 콤보를 할 경우 채워지는 게이지의 양
CRASH_DEC_VALUE_IN_NORMAL = 0 -- 노멀모드에서 맞았을 때 깎이는 게이지의 양. 반드시 음수여야 함.
CRASH_DEC_VALUE_IN_ZERO = -50 -- 제로모드에서 맞았을 때 깎이는 게이지의 양. 반드시 음수여야 함.
HORIZ_SPEED = 5.2 	-- 좌우 이동속도
STUN_HORIZ_SPEED = 3 -- 스턴시 좌우 이동 속도
OBSTACLES_OF_LINE = 4 -- 한줄당 생성할 장애물수
flagGap = 120
flagXMinDistance = 100
flagXMaxDistance = 250

function updateBalance(currentCombo, bZero, timeAfterZero)
	if bZero == false then
		MAX_SPEED=5
		MIN_SPEED=3
		OBSTACLES_OF_LINE=4
		MIN_CREATION_DISTANCE=100
		MAX_CREATION_DISTANCE=140
		OBSTACLES_OF_LINE=4
		-- timeAfterZero=0
		PLAYERWIDTH = 25
		flagGap=120
		FLAG_MAX_CREATION_DISTANCE=600
		FLAG_MIN_CREATION_DISTANCE=700
	else
        if(bZero and currentCombo>14) then
        	OBSTACLES_OF_LINE=8
        	PLAYERWIDTH=10
        	flagGap = 60
        elseif(bZero and currentCombo>10) then
	        OBSTACLES_OF_LINE=7
	        PLAYERWIDTH=10 
	        flagGap = 60
        elseif(bZero and currentCombo>8) then
	        OBSTACLES_OF_LINE=7
	        PLAYERWIDTH=13
	        flagGap = 70
        elseif(bZero and currentCombo>6) then
	        OBSTACLES_OF_LINE=6
	        PLAYERWIDTH=16
	        flagGap = 80
        elseif(bZero and currentCombo>4) then
	         OBSTACLES_OF_LINE=6
	         PLAYERWIDTH=19
	         flagGap = 90
        elseif(bZero and currentCombo>2) then
	         OBSTACLES_OF_LINE=5
	         PLAYERWIDTH=22
	         flagGap = 100
        else
            OBSTACLES_OF_LINE=5
            PLAYERWIDTH=25
            flagGap = 100
        end
		if(timeAfterZero>40) then
			--OBSTACLES_OF_LINE=5
			MAX_SPEED=10
			MIN_SPEED=6.5
			--MIN_CREATION_DISTANCE=100
			--MAX_CREATION_DISTANCE=340
			FLAG_MAX_CREATION_DISTANCE=1200
			FLAG_MIN_CREATION_DISTANCE=1100
			PLAYERWIDTH = PLAYERWIDTH - 5
		elseif(timeAfterZero>30) then
			--OBSTACLES_OF_LINE=5
			MAX_SPEED=9
			MIN_SPEED=6
			--MIN_CREATION_DISTANCE=100
			--MAX_CREATION_DISTANCE=300
			FLAG_MAX_CREATION_DISTANCE=1100
			FLAG_MIN_CREATION_DISTANCE=1000
			PLAYERWIDTH = PLAYERWIDTH - 4
		elseif(timeAfterZero>20) then
			--OBSTACLES_OF_LINE=5
			MAX_SPEED=8
			MIN_SPEED=5.8
			--MIN_CREATION_DISTANCE=100
			--MAX_CREATION_DISTANCE=260
			FLAG_MAX_CREATION_DISTANCE=1000
			FLAG_MIN_CREATION_DISTANCE=900
			PLAYERWIDTH = PLAYERWIDTH - 3
		elseif(timeAfterZero>10) then
			--OBSTACLES_OF_LINE=5
			MAX_SPEED=7.5
			MIN_SPEED=5.5
			--MIN_CREATION_DISTANCE=100
			--MAX_CREATION_DISTANCE=220
			FLAG_MAX_CREATION_DISTANCE=900
			FLAG_MIN_CREATION_DISTANCE=800
			PLAYERWIDTH = PLAYERWIDTH - 2
		elseif(timeAfterZero>5) then
			--OBSTACLES_OF_LINE=5
			MAX_SPEED=7
			MIN_SPEED=5.2
	--		MIN_CREATION_DISTANCE=100
	--		MAX_CREATION_DISTANCE=180
			FLAG_MAX_CREATION_DISTANCE=800
			FLAG_MIN_CREATION_DISTANCE=700
			PLAYERWIDTH = PLAYERWIDTH - 1
		else
			MAX_SPEED=6.5
			MIN_SPEED=5
			FLAG_MAX_CREATION_DISTANCE=800
			FLAG_MIN_CREATION_DISTANCE=700
		end
    end

end