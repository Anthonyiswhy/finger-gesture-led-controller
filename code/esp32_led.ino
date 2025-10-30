import cv2
import time
import requests
import mediapipe as mp

ESP32_IP = "http://192.168.1.25"  # Change to your ESP32's IP address if different

# Finger tip and joint mappings
fingertips = {"thumb": 4, "index": 8, "middle": 12, "ring": 16, "pinky": 20}
fingerjoints = {"thumb": 2, "index": 6, "middle": 10, "ring": 14, "pinky": 18}

# Track previous state
prev_states = {f: None for f in fingertips}

# MediaPipe setup
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1)
mp_draw = mp.solutions.drawing_utils
cap = cv2.VideoCapture(0)

def send_all_states(states):
    try:
        # Format: /leds?thumb=1&index=0&...
        query = "&".join([f"{f}={int(states[f])}" for f in states])
        url = f"{ESP32_IP}/leds?{query}"
        resp = requests.get(url, timeout=0.5)
        print("Sent states:", states, "→ Response:", resp.text)
    except Exception as e:
        print("Error sending states:", e)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.flip(frame, 1)
    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = hands.process(rgb)

    if results.multi_hand_landmarks:
        hand_landmarks = results.multi_hand_landmarks[0]
        mp_draw.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)

        current_states = {}
        for finger in fingertips:
            tip = hand_landmarks.landmark[fingertips[finger]]
            joint = hand_landmarks.landmark[fingerjoints[finger]]

            if finger == "thumb":
                # Right hand: thumb is extended if tip is left of joint
                is_up = tip.x < joint.x
            else:
                # Other fingers: extended if tip is above joint
                is_up = tip.y < joint.y

            current_states[finger] = is_up

        # Only send if something changed
        if current_states != prev_states:
            send_all_states(current_states)
            prev_states = current_states.copy()

    cv2.imshow("Gesture Control", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    time.sleep(0.05)  # ~20 FPS

cap.release()
cv2.destroyAllWindows()

