 #include "ultrasonic.h"
    
    ultrasonic::ultrasonic(PinName trigPin, PinName echoPin, float updateSpeed, float timeout):_trig(trigPin), _echo(echoPin)
    {
        _updateSpeed = updateSpeed;
        _timeout = timeout;
    }
    
    ultrasonic::ultrasonic(PinName trigPin, PinName echoPin, float updateSpeed, float timeout, void onUpdate(int))
    :_trig(trigPin), _echo(echoPin)
    {
        _onUpdateMethod=onUpdate;
        _updateSpeed = updateSpeed;
        _timeout = timeout;
        _t.start ();
    }
    void ultrasonic::_startT()
    { 
        if(_t.read()>600)
        {
            _t.reset ();
        }
        start = _t.read_us ();
    }
        
    void ultrasonic::_updateDist()
    {
        end = _t.read_us ();
        done = 1;
        _distance = (end - start)/6;       
        _tout.detach();
        _tout.attach(Callback<void()>(this,&ultrasonic::_startTrig), _updateSpeed);   
    }
    void ultrasonic::_startTrig(void)
    {
            _tout.detach();
            _trig=1;             
            wait_us(10);        
            done = 0;            
            _echo.rise(Callback<void()>(this,&ultrasonic::_startT));   
            _echo.fall(Callback<void()>(this,&ultrasonic::_updateDist));
            _echo.enable_irq ();
            _tout.attach(Callback<void()>(this,&ultrasonic::_startTrig),_timeout);
            _trig=0;                     
    }
    
    int ultrasonic::getCurrentDistance(void)
    {
        return _distance;
    }
    void ultrasonic::pauseUpdates(void)
    {
        _tout.detach();
        _echo.rise(NULL);
        _echo.fall(NULL);
    }
    void ultrasonic::startUpdates(void)
    {
        _startTrig();
    }
    void ultrasonic::attachOnUpdate(void method(int))
    {
        _onUpdateMethod = method;
    }
    void ultrasonic::changeUpdateSpeed(float updateSpeed)
    {
        _updateSpeed = updateSpeed;
    }
    float ultrasonic::getUpdateSpeed()
    {
        return _updateSpeed;
    }
    int ultrasonic::isUpdated(void)
    {
        //printf("%d", done);
        d=done;
        done = 0;
        return d;
    }
    void ultrasonic::checkDistance(void)
    {
        if(isUpdated())
        {
            (*_onUpdateMethod)(_distance);
        }
    }
