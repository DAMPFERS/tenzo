#include <main.h>
#include <oled_RVV.h>
#include <encoder_RVV.h>
#include <motor_control_RVV.h>
#include <gui.h>
#include <serial.h>


void unpacking(unsigned char pack);


/* переменные для хранения состояния нажатия кнопки (защита от множественных срабатываний кнопок) */
unsigned char old_button_state = 0x00; 
unsigned char new_button_state = 0x00;

double delta_angle = STEP_ANGLE * 50;   // шаг угла поворота (целое количество поворотов умножить на минимальный угол поворота (угол шага)) 

unsigned char state_flag = STATE_SELECT;  // флаг состояния выбора меню
unsigned char work_flag = 0;              // флаг режима работы


signed char index_rotation = index_velocity;   // индекс скорости поворота (может быть отрицательным (показывает направление вращения)), из него пересчитывается индекс для масива допустимых скоростей (motor.c)

double angle_old = 0.0;  // старое значение угла поворота
double angle_new = 0.0;  // новое значение угла поворота

unsigned char position_select = ANGLE;   // хранит положение ползунка выбора (для отрисовки)

float speed_old = 0.0;   // старое значение скорости вращения
float speed_new = 0.0;   // новое значение скорости вращения

unsigned char end_positinon_NOW;
unsigned char end_positinon_NEW;


int main(void) { 
  
  
  WRITE_MENU();

  end_positinon_NOW = write_NOW(POSITION_NOW, PAGE_NOW);
  end_positinon_NEW = write_NEW(POSITION_NEW, PAGE_NEW);

  /* заполнение нулевым значением */
  write_number(0.0, end_positinon_NOW, PAGE_NOW);  
  write_number(0.0, end_positinon_NEW, PAGE_NEW);
  
  write_selection(position_select);

  motorInit(PB0,PB1,PB2,PB3);   // инициализация пинов для работы с мотором
  encoderInit(LEFT_ENCODER_PIN, RIGHT_ENCODER_PIN); // инициализация пинов для работы с энкодером
  UART_Init(9600);
  
  unsigned char received_char;

  while (1) {
    
    if (encoder_state == LEFT){ // если поворот энкодера против часовой стрелки
      encoder_state = 0;        // обнуление (сброс) состояния поворота энкодера 

      if (state_flag == STATE_SELECT){    // если состояние системы стоит на выборе режима
        clear_selection(position_select); // стирание старого флажка на старой позиции 
        position_select = position_select > ANGLE ? position_select - 1 : STOP; // установка состояния позиции на новой итерации
        write_selection(position_select); // рисование флажка на новой позиции
      }
        
      else if (state_flag == STATE_SET){  // если состояние системы стоит на выборе значения
        /* стирание предыдущих значений значений */
        write_numberClear(end_positinon_NOW, PAGE_NOW);   
        write_numberClear(end_positinon_NEW, PAGE_NEW);   
        
        switch (position_select)
        {
        case ANGLE: // если состояние позиции стоит на выборе угла поворота
          write_number(angle_old, end_positinon_NOW, PAGE_NOW);                 // отрисовка старого значения
          angle_new = angle_new < delta_angle ? 0.0 : angle_new - delta_angle;  // установка нового угла
          write_number(angle_new, end_positinon_NEW, PAGE_NEW);                 // отрисовка нового значения
          break;

        case ROTATION: // если состояние позиции стоит на выборе скорости вращения
          write_number(speed_old, end_positinon_NOW, PAGE_NOW); // отрисовка старого значения
          
          index_rotation = index_rotation != -19 ? index_rotation - 1 : index_rotation; // декремент индекса скорости поворота

          /* пересчет индекса допустимых значений скоростей и установка флага направления вращения (motor.c) */
          if (index_rotation < 0){
            index_velocity = -1 * index_rotation - 1;
            flag_start_timer_motor = DOWN;
            speed_new = -velocity_value_settings[index_velocity];
          }
          else{
            index_velocity = index_rotation;
            flag_start_timer_motor = UP;
            speed_new = velocity_value_settings[index_velocity];
          } 

          // index_velocity = index_velocity == 0 ? 18 : index_velocity - 1;     
          // speed_new = speed_new < delta_speed ? 0.0 : speed_new - delta_speed;
          // speed_new = velocity_value_settings[index_velocity];
          write_number(speed_new, end_positinon_NEW, PAGE_NEW); // отрисовка значения новой скорости
          break;

        case OSCILATION: // если состояние позиции стоит на выборе угла осциляции
          write_number(0, end_positinon_NOW, PAGE_NOW);                 // отрисовка старого значения
          oscil_angle -= delta_angle;                                             // установка нового угла
          write_number(oscil_angle, end_positinon_NEW, PAGE_NEW);                 // отрисовка нового значения
          break;
      }

      }
    }


    else if(encoder_state == RIGHT){ // если поворот энкодера по часовой стрелки
      encoder_state = 0; // сброс состояния поворота энкодера
      if (state_flag == STATE_SELECT){
        clear_selection(position_select);
        position_select = position_select < STOP ? position_select + 1 : ANGLE;
        write_selection(position_select);
        
      }
        
      else if (state_flag == STATE_SET){
        write_numberClear(end_positinon_NOW, PAGE_NOW);
        write_numberClear(end_positinon_NEW, PAGE_NEW);

        switch (position_select)
        {
        case ANGLE:
          write_number(angle_old, end_positinon_NOW, PAGE_NOW);                 // отрисовка старого значения
          angle_new += delta_angle;
          write_number(angle_new, end_positinon_NEW, PAGE_NEW);                 // отрисовка нового значения
          break;
        
        case ROTATION:
          write_number(speed_old, end_positinon_NOW, PAGE_NOW);
          
          index_rotation = index_rotation != 18 ? index_rotation + 1 : index_rotation;
          if (index_rotation < 0){
            index_velocity = -1 * index_rotation - 1;
            flag_start_timer_motor = DOWN;
            speed_new = -velocity_value_settings[index_velocity];
          }
          else{
            index_velocity = index_rotation;
            flag_start_timer_motor = UP;
            speed_new = velocity_value_settings[index_velocity];
          } 

          
          // index_velocity = index_velocity == 18 ? 0 : index_velocity + 1;
          // speed_new = velocity_value_settings[index_velocity];
          write_number(speed_new, end_positinon_NEW, PAGE_NEW);
          break;

        case OSCILATION: 
          write_number(0, end_positinon_NOW, PAGE_NOW);                 // отрисовка старого значения
          oscil_angle += delta_angle;                                             // установка нового угла
          write_number(oscil_angle, end_positinon_NEW, PAGE_NEW);                 // отрисовка нового значения
          break;

        }
        
      }
    }
    
     
    /*___________ обработка нажатия кнопки___________________________ */
    if (!USART_Read(&received_char))
      new_button_state = buttonReading(BUTTON_PIN);            // считывание нового состояния кнопки 
    else unpacking(received_char);

    if ((new_button_state == 1) && (old_button_state == 0)){ // если происходит смена состояния с 1 в 0 (короткое нажатие)
      
      if (state_flag == STATE_SELECT){    // если нажатие кнопки произошло в режиме STATE_SELECT (выбора) 
        switch (position_select){          // смотрим позицию меню

        case ANGLE:
          write_FOOTNOTE_Clear();
          write_FOOTNOTE(ANGLE);
          work_flag = WORK_ANGLE;
          state_flag = STATE_SET;
          break;

        case ROTATION:
          write_FOOTNOTE_Clear();
          write_FOOTNOTE(ROTATION);
          work_flag = WORK_SPEED;
          state_flag = STATE_SET;
          break;
        
        case OSCILATION:
          write_FOOTNOTE_Clear();
          write_FOOTNOTE(OSCILATION);
          work_flag = WORK_OSCIL;
          state_flag = STATE_SET;
          break;

        case START:
          switch (work_flag){
          case WORK_ANGLE:
            setAngle(angle_new);
            angle_old = angle_new;
            write_numberClear(end_positinon_NOW, PAGE_NOW);
            write_number(angle_old, end_positinon_NOW, PAGE_NOW);
            break;
          
          case WORK_SPEED:
            setSpeed();
            run_flag = 1;
            speed_old = speed_new;
            write_numberClear(end_positinon_NOW, PAGE_NOW);
            write_number(speed_new, end_positinon_NOW, PAGE_NOW);
            break;

          case WORK_OSCIL:
            oscil_flag = 1;
            break;
          }
          break;

        case STOP:
          run_flag = 0;
          switch (work_flag)
          {
          case WORK_ANGLE:
            now_angle = new_angle;
            angle_old = now_angle;
            write_numberClear(end_positinon_NOW, PAGE_NOW);
            write_number(angle_old, end_positinon_NOW, PAGE_NOW);
            break;
          
          case WORK_SPEED:
            run_flag = 0;
            new_angle = now_angle;
            angle_old = now_angle;
            write_numberClear(end_positinon_NOW, PAGE_NOW);
            write_number(angle_old, end_positinon_NOW, PAGE_NOW);
            break;

          case WORK_OSCIL:
            oscil_flag = 0;
            break;

          }
          break;
        }
        
      }
        
      else if (state_flag == STATE_SET)  
       state_flag = STATE_SELECT;
    }
    old_button_state = new_button_state;

    
  }

}



void unpacking(unsigned char pack){

  if (pack & (1 << 7)){ // START

    unsigned char command = (pack >> 5) & 0b11;
    new_button_state = 1;
    state_flag = STATE_SELECT;
    position_select = START;

    switch (command)
    {
    case 0b00: // angle
      write_FOOTNOTE_Clear();
      write_FOOTNOTE(ANGLE);
      work_flag = WORK_ANGLE;
      angle_new = (pack & 0x1f) * STEP_ANGLE * 114;  // установка нового угла
      break;

    case 0b01: // rotate
      write_FOOTNOTE_Clear();
      write_FOOTNOTE(ROTATION);
      work_flag = WORK_SPEED;
      flag_start_timer_motor = UP;
      speed_new = velocity_value_settings[pack & 0x1f];
      index_velocity = pack & 0x1f;
      break;

    case 0b10: // -rotate
      write_FOOTNOTE_Clear();
      write_FOOTNOTE(ROTATION);
      work_flag = WORK_SPEED;
      flag_start_timer_motor = DOWN;
      speed_new = -velocity_value_settings[pack & 0x1f];
      index_velocity = pack & 0x1f;
      break;

    case 0b11: // oscillation
      write_FOOTNOTE_Clear();
      write_FOOTNOTE(OSCILATION);
      work_flag = WORK_OSCIL;
      oscil_angle = (pack & 0x1f) * STEP_ANGLE * 114; 
      break;
    }
  }
  else{ // STOP
    new_button_state = 1;
    state_flag = STATE_SELECT;
    position_select = STOP;
  }

  return;
}

