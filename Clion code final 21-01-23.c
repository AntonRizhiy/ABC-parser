#include <stdio.h>

#define UART_BUFFER_SIZE (30)
#define LUT (7)
#define TIMER_PERIOD (9600) /* Timer period default value (300ms) */
#define FALSE (0)
#define TRUE (1)


/* ************************************************************************** */
/* *************************** GLOBAL VARIABLES ***************************** */
/* ************************************************************************** */
int idx_lut = 0;
int duration_multiplier = 1;
char octave = '!';
/* ****************************** Pointers ********************************** */
int *pointer_idx_lut = &idx_lut;
char *pointer_octave = &octave;
int *pointer_duration_multiplier = &duration_multiplier;
/* ************************************************************************** */
/* ***************************** UART BUFFER ******************************** */
char UartBuffer[UART_BUFFER_SIZE] = {'c','c','c','2','c','c','c','2','c','e','A','B','c','4','d','d','d','d','d','c','c','c','c','B','B','c','B','2','e','2'};

int UartBufferIdx = 0;
/* ************************************************************************** */

typedef enum{
    NEXT_NOTE_OK,    // 0
    NEXT_NOTE_END,   // 1
    NEXT_NOTE_ERROR  // 2
} NextNoteType;

/* ***************************************** NOTES LUT ****************************************** */

char NoteNames_3_octave[LUT] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};
char NoteNames_4_octave[LUT] = {'c', 'd', 'e', 'f', 'g', 'a', 'b'};
int Note_Freq_3_octave[LUT] = {262, 294, 330, 349, 392, 440, 494};
int Note_Freq_4_octave[LUT] = {523, 587, 659, 699, 784, 880, 988};
/* ********************************************************************************************** */
void Buffer_Print()
{
    printf("\n My buffer: \t");
    for(int i = 0; i < UART_BUFFER_SIZE; i++)
    {
        if(i < 10)
        {
            printf("%c|", UartBuffer[i]);
        }
        else
        {
            printf("%c |", UartBuffer[i]);
        }
    }

    printf("\n Idx buffer:\t");
    for(int i = 0; i < UART_BUFFER_SIZE; i++)
    {
        printf("%d|", i);
    }
    printf("\n");
}

/* ********************************** ABC PROCESSOR DESCRIPTOR ********************************** */
typedef struct {
    int Size_receiv_file; /* Size of received ABC file */
 	int Current_position_File; /* Current position of processing ABC file */
 	int Details_repeat_melody; /* Details of repeat melody fragment */
}AbcProcessorDescriptorType;
AbcProcessorDescriptorType AbcProcessorDescriptor;
/* ********************************************************************************************** */

typedef struct
{
    int frequency; /* Value frequency note for generation signal in PWM */
    int duration_note; /* Note duration for Timer setting */
}NoteDescriptorType;
NoteDescriptorType NoteDescriptor; /* Use in parser for take parameters note */
/* ********************************************************************************************** */
void Update_Descriptor_Note(_Bool check_duration)
{
    if(octave == 't' && check_duration == FALSE)
    {
        NoteDescriptor.frequency = Note_Freq_4_octave[idx_lut];
        NoteDescriptor.duration_note = TIMER_PERIOD;
    }

    else if(octave == 't' && check_duration == TRUE)
    {
        NoteDescriptor.frequency = Note_Freq_4_octave[idx_lut];
        NoteDescriptor.duration_note = TIMER_PERIOD * duration_multiplier;
    }

    else if(octave == 'b' && check_duration == FALSE)
    {
        NoteDescriptor.frequency = Note_Freq_3_octave[idx_lut];
        NoteDescriptor.duration_note = TIMER_PERIOD;
    }

    else if(octave == 'b' && check_duration == TRUE)
    {
        NoteDescriptor.frequency = Note_Freq_3_octave[idx_lut];
        NoteDescriptor.duration_note = TIMER_PERIOD * duration_multiplier;
    }
    else
    {
        NoteDescriptor.frequency = 0;
        NoteDescriptor.duration_note = 0;
    }
}
/* ********************************************************************************************** */
_Bool Check_Buffer_Index_Range(int arg)
{
    if (arg < UART_BUFFER_SIZE && arg > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/* ********************************************************************************************** */
_Bool Note_Search_Func(int arg)
{
    UartBufferIdx = arg;
    _Bool result = TRUE;
    _Bool check_note_change = TRUE;

    while(check_note_change == TRUE)
    {
        for(int lut_top = 0; lut_top < LUT; lut_top++)
        {
            if(UartBuffer[UartBufferIdx] == NoteNames_4_octave[lut_top])
            {
                *pointer_idx_lut = lut_top;
                *pointer_octave = 't';
                AbcProcessorDescriptor.Current_position_File = UartBufferIdx;

                result = TRUE;
                return result;
            }
        }

        for(int lut_bottom = 0; lut_bottom < LUT; lut_bottom++)
        {
            if(UartBuffer[UartBufferIdx] == NoteNames_3_octave[lut_bottom])
            {
                *pointer_idx_lut = lut_bottom;
                *pointer_octave = 'b';
                AbcProcessorDescriptor.Current_position_File = UartBufferIdx;

                result = TRUE;
                return result;
            }
        }

        UartBufferIdx++;
        if(Check_Buffer_Index_Range(UartBufferIdx) == FALSE)
        {
            check_note_change = FALSE;
            result = FALSE;
            return result;
        }
    }
    return result;
}
/* ********************************************************************************************** */
_Bool Check_Number_After_Note(int arg)
{
    if(UartBuffer[arg] == '2')
    {
        *pointer_duration_multiplier = 2;
        AbcProcessorDescriptor.Current_position_File = arg++;
        return TRUE;
    }
    else if(UartBuffer[arg] == '3')
    {
        *pointer_duration_multiplier = 3;
        AbcProcessorDescriptor.Current_position_File = arg++;
        return TRUE;
    }
    else if(UartBuffer[arg] == '4')
    {
        *pointer_duration_multiplier = 4;
        AbcProcessorDescriptor.Current_position_File = arg++;
        return TRUE;
    }
    else if(UartBuffer[arg] == '5')
    {
        *pointer_duration_multiplier = 5;
        AbcProcessorDescriptor.Current_position_File = arg++;
        return TRUE;
    }
    else if(UartBuffer[arg] == '6')
    {
        *pointer_duration_multiplier = 6;
        AbcProcessorDescriptor.Current_position_File = arg++;
        return TRUE;
    }
    else if(UartBuffer[arg] == '7')
    {
        *pointer_duration_multiplier = 7;
        AbcProcessorDescriptor.Current_position_File = arg++;
        return TRUE;
    }
    else if(UartBuffer[arg] == '8')
    {
        *pointer_duration_multiplier = 8;
        AbcProcessorDescriptor.Current_position_File = arg++;
        return TRUE;
    }
    else if(UartBuffer[arg] == '9')
    {
        *pointer_duration_multiplier = 9;
        AbcProcessorDescriptor.Current_position_File = arg++;
        return TRUE;
    }
    else
    {
        *pointer_duration_multiplier = 1;
        AbcProcessorDescriptor.Current_position_File = arg;
        return FALSE;
    }
}
/* ********************************************************************************************** */
NextNoteType GetNextNote()
{
    NextNoteType return_value;

    int idx = AbcProcessorDescriptor.Current_position_File;

    _Bool try_note = Note_Search_Func(idx);

    idx = AbcProcessorDescriptor.Current_position_File + 1;

    _Bool try_number_after_note = Check_Number_After_Note(idx);

    if (try_note == TRUE)
    {
        return_value = NEXT_NOTE_OK;
        Update_Descriptor_Note(try_number_after_note);
        return return_value;
    }

    else if (try_note == FALSE)
    {
        return_value = NEXT_NOTE_END;
        return return_value;
    }

    else
    {
        return_value = NEXT_NOTE_ERROR;
        return return_value;
    }
}
/* ********************************************************************************************** */
int main()
{
    Buffer_Print();

    UartBufferIdx = 0;
    AbcProcessorDescriptor.Current_position_File = 0;

    for(UartBufferIdx; UartBufferIdx < UART_BUFFER_SIZE; )
    {
        NextNoteType Ret = GetNextNote();

        if(Ret == NEXT_NOTE_END)
        {
            printf("|-------------------------------------------------------------------------------| \n");
            printf("|-  All notes successful parse. The NEXT_NOTE_END result return. All right :)  -| \n");
            printf("|-------------------------------------------------------------------------------| \n");

        }
        else
        {
            if(octave == 't')
            {
                printf(" N: %d | Ret = %d", UartBufferIdx, Ret);
                printf(" | Note is: %c", NoteNames_4_octave[idx_lut]);
                printf(" | frequency = %d, duration = %d \n", NoteDescriptor.frequency, NoteDescriptor.duration_note);
            }

            if(octave == 'b')
            {
                printf(" N: %d | Ret = %d", UartBufferIdx, Ret);
                printf(" | Note is: %c", NoteNames_3_octave[idx_lut]);
                printf(" | frequency = %d, duration = %d \n", NoteDescriptor.frequency, NoteDescriptor.duration_note);
            }
        }
    }
    return 0;
}
