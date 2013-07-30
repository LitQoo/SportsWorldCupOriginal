PLAYER_SPEED = 4
CHAR_POSITION = 100
MIN_TURNING_SPACING = 150 -- 꼭지점 사이의 x 거리
MAX_TURNING_SPACING = 500
MIN_BUILDING_Y = 30 -- -- 빌딩 높이 
MAX_BUILDING_Y = 170
MIN_PASS_SPACING = 200 ---- 통과 간격
MAX_PASS_SPACING = 300
ONCE_CREATING_SIZE = 480 ---- 중간 장애물 거리
MIN_MIDDLE_DISTANCE = 300
MAX_MIDDLE_DISTANCE = 500
MIN_PASS_MARGIN = 60
MIN_TURNING_SPACING = 5



ZERO_DEC_VALUE = -0.05
INC_VALUE_IN_ZERO = 5
INC_VALUE_IN_NORMAL = 13
CRASH_DEC_VALUE_IN_NORMAL = 0
CRASH_DEC_VALUE_IN_ZERO = -40
SCORE_PER_BUILDING = 300 -- 건물 하나 지날 때...
COIN_HEIGHT = 40 -- 동전 높이.


function score(currentCombo)
    if currentCombo<20 then
        return currentCombo * 2000
    else
        return 40000
    end
end


function updateBalance(currentCombo, bZero, timeAfterZero)
    if bZero>0 then
        PLAYER_SPEED = PLAYER_SPEED+0.001
        if PLAYER_SPEED>8 then
            PLAYER_SPEED=8
        end
        
        MIN_PASS_SPACING = 200 - (currentCombo * 3)
        MAX_PASS_SPACING = 300 - (currentCombo * 1.5)
        if MIN_PASS_SPACING < 100 then
            MIN_PASS_SPACING = 100
            MAX_PASS_SPACING = 250
        end
        
        COIN_HEIGHT = MIN_PASS_SPACING-20
        
    else
        MIN_PASS_SPACING = 200
        MAX_PASS_SPACING = 300
        PLAYER_SPEED = 4
        COIN_HEIGHT = 40
    end
    

end
