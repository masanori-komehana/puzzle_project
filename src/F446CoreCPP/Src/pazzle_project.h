
#ifndef SRC_PAZZLE_PROJECT_H /* beginning of include guard SRC_PAZZLE_PROJECT_H */
#define SRC_PAZZLE_PROJECT_H

/* Includes ------------------------------------------------------------------*/


/* Private includes ----------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/


#define SW_NUM 7
#define TICKS_1S 3200


enum TGeneralFlgs {
    // 画面データ要求
    CLS,
    // 区間ごとに立てるフラグ
    EN_1s,
    EN_10ms,
    // 何かのスイッチ押された
    SW_POSEDGE,
    // USBになんか来た
    RECV_USART,
};



/* Exported constants --------------------------------------------------------*/



/* Exported macro ------------------------------------------------------------*/


#define FLG(flg_val) (((unsigned int)0x00000001)<<(flg_val))
#define HAS_FLG(flg_dat, flg_val) flg_dat & FLG(flg_val)
#define CLR_FLG(flg_dat, flg_val) flg_dat &= ~FLG(flg_val)
#define SET_FLG(flg_dat, flg_val) flg_dat |= FLG(flg_val)


/* Exported functions prototypes ---------------------------------------------*/

/* Private defines -----------------------------------------------------------*/




#endif /* end of include guard: SRC_PAZZLE_PROJECT_H */
