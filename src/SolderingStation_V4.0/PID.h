//КЛАСС ПИД-РЕГУЛЯТОРА

class PID {
  private:
    float _frTime;
    uint64_t time;
    float prevError;
    float P;
    float _lowerLim = -1000;
    float I;
    float _upperLim = 1000;
    float D;
  
  public:
    PID(float frTime){
      _frTime = frTime;
    }

    boolean available(){
      if(millis() - time > _frTime) return true;
      else return false;
    }

    void limitIntegral(float lowerLim, float upperLim){
      _lowerLim = lowerLim;
      _upperLim = upperLim;
    }
    
    float calculatePID(float kP, float kI, float kD, float err){
      if(millis() - time > (_frTime)){
        P = kP * err;
        I += kI * err;
        I = constrain(I, _lowerLim, _upperLim);
        D = kD * ((err - prevError));
        prevError = err;
        time = millis();
      }
      return P + I + D;
    }
  
};
