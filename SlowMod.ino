#include "ComputerCard.h"

/// Dual sample and hold

/// Updates audio output to take the value of the audio input
/// only when the corresponding pulse input has a rising edge.

/// If audio input not connected, sample random noise instead.

/// If pulse input not connected, update output every sample,
/// (tracking audio input if connected, or producing white noise
/// if audio input not connected.)

// Slow LFO

/*
looking for
sin(omega1 t)
and 
sin(omega2 t)

such that at t=T, second sine has done one more phase than first.

That is,

omega1 T + 2pi = omega2 T
omega2 = omega1 + 2pi/T

Now, index here is 21-bit ~= 2 million
Sample rate 48kHz
So incrementing one per sample gives 2^21/48000Hz = 44s loop
Let's just increment a 32-bit unsigned, let it wrap

 */

int32_t voct_vals[341] = { 314964268, 315605144, 316247323, 316890810, 317535606, 318181713, 318829136, 319477876, 320127936, 320779318, 321432026, 322086062, 322741429, 323398129, 324056166, 324715542, 325376259, 326038320, 326701729, 327366488, 328032599, 328700066, 329368890, 330039076, 330710625, 331383541, 332057826, 332733483, 333410515, 334088924, 334768714, 335449887, 336132446, 336816394, 337501733, 338188467, 338876599, 339566130, 340257065, 340949405, 341643155, 342338315, 343034891, 343732883, 344432296, 345133132, 345835394, 346539085, 347244208, 347950766, 348658761, 349368197, 350079076, 350791402, 351505177, 352220405, 352937088, 353655229, 354374831, 355095898, 355818432, 356542436, 357267913, 357994867, 358723299, 359453214, 360184614, 360917502, 361651881, 362387755, 363125126, 363863998, 364604372, 365346254, 366089644, 366834548, 367580967, 368328905, 369078365, 369829350, 370581862, 371335907, 372091485, 372848601, 373607257, 374367457, 375129204, 375892500, 376657350, 377423757, 378191722, 378961250, 379732344, 380505008, 381279243, 382055053, 382832443, 383611414, 384391970, 385174114, 385957850, 386743180, 387530108, 388318638, 389108772, 389900514, 390693867, 391488834, 392285418, 393083624, 393883453, 394684911, 395487998, 396292720, 397099080, 397907080, 398716724, 399528016, 400340958, 401155555, 401971809, 402789724, 403609303, 404430550, 405253468, 406078060, 406904330, 407732282, 408561918, 409393242, 410226258, 411060969, 411897378, 412735489, 413575305, 414416830, 415260068, 416105021, 416951694, 417800089, 418650211, 419502062, 420355647, 421210969, 422068031, 422926837, 423787390, 424649694, 425513753, 426379570, 427247149, 428116493, 428987606, 429860492, 430735153, 431611595, 432489820, 433369831, 434251634, 435135230, 436020625, 436907821, 437796822, 438687632, 439580255, 440474694, 441370953, 442269035, 443168945, 444070686, 444974262, 445879677, 446786934, 447696036, 448606989, 449519795, 450434459, 451350984, 452269373, 453189631, 454111762, 455035769, 455961656, 456889428, 457819087, 458750637, 459684083, 460619429, 461556677, 462495833, 463436900, 464379881, 465324781, 466271604, 467220353, 468171033, 469123648, 470078200, 471034695, 471993136, 472953528, 473915873, 474880177, 475846443, 476814674, 477784876, 478757053, 479731207, 480707343, 481685466, 482665579, 483647686, 484631791, 485617899, 486606014, 487596139, 488588278, 489582437, 490578618, 491576826, 492577066, 493579340, 494583654, 495590012, 496598417, 497608874, 498621387, 499635961, 500652599, 501671305, 502692084, 503714940, 504739878, 505766901, 506796014, 507827220, 508860525, 509895933, 510933447, 511973073, 513014813, 514058674, 515104658, 516152771, 517203017, 518255399, 519309923, 520366592, 521425412, 522486386, 523549519, 524614815, 525682278, 526751914, 527823726, 528897719, 529973898, 531052266, 532132828, 533215589, 534300553, 535387725, 536477109, 537568709, 538662531, 539758579, 540856856, 541957368, 543060120, 544165115, 545272359, 546381856, 547493610, 548607627, 549723910, 550842464, 551963295, 553086406, 554211802, 555339489, 556469470, 557601750, 558736334, 559873227, 561012433, 562153957, 563297803, 564443977, 565592484, 566743327, 567896512, 569052043, 570209926, 571370165, 572532764, 573697729, 574865065, 576034775, 577206866, 578381342, 579558207, 580737467, 581919127, 583103191, 584289664, 585478552, 586669858, 587863589, 589059748, 590258342, 591459374, 592662850, 593868775, 595077154, 596287991, 597501292, 598717062, 599935306, 601156029, 602379235, 603604930, 604833120, 606063808, 607297001, 608532703, 609770919, 611011654, 612254915, 613500705, 614749029, 615999894, 617253304, 618509265, 619767781, 621028858, 622292501, 623558715, 624827505, 626098877, 627372836, 628649388 };

// Takes integers 0-4095 from CV in
// Returns exponential values corresponding to volts per octave
// Max frequency about 20kHz, should aim for 2^30 ~ 1 billion, so want max number about 500 million
// About 455 values per octave
// Starts with  314964268 because this  314964268/2^32 * 48kHz = (3512Hz) 3 octaves above A440
int32_t ExpVoct(int32_t in) {
  if (in > 4091) in = 4091;  // limit to 12 oct;
  int32_t oct = in / 341;
  int32_t suboct = in % 341;

  return voct_vals[suboct] >> (12 - oct);
}

#include <cmath>
class SlowMod : public ComputerCard {
public:
  static constexpr int npts = 8192;
  int32_t sinevals[npts];

  void reset() {
    // start LFOs at different phases
    phase1 = 0;
    phase2 = 2 << 29;
    phase3 = (2 << 30) + (2 << 29);
    phase4 = 2 << 30;
  }
  uint32_t phase1, phase2, phase3, phase4;
  SlowMod() {
    for (int i = 0; i < npts; i++) {
      // just shy of 2^22 * sin
      sinevals[i] = 2048 * 2040 * sin(2 * i * M_PI / double(npts));
    }

    reset();
  }
  // Given 21-bit index x, return 2^22 * sin(y) where y = x/2^21
  int32_t sinval(int32_t x) {
    x &= 0x001FFFFF;       // wrap at 21 bits = 13+8 bits
    int32_t r = x & 0xFF;  //
    x >>= 8;               // x now 13-bit number, 0-8191
    int32_t s1 = sinevals[x];
    int32_t s2 = sinevals[(x + 1) & 0x1FFF];
    return (s2 * r + s1 * (256 - r)) >> 8;
  }

  void SetAudio1(int32_t cv_val) {
    static int32_t error = 0;
    cv_val += 524288;
    uint32_t truncated_cv_val = (cv_val - error) & 0xFFFFFF00;
    error += truncated_cv_val - cv_val;
    int16_t val = int32_t(truncated_cv_val >> 8) - 2048;
    if (Connected(Input::Audio1)) {
      LedOn(0, true);
      AudioOut1((val * AudioIn1()) >> 12);
    } else {
      LedOn(0, false);
      AudioOut1(val);
    }
  }
  void SetAudio2(int32_t cv_val) {
    static int32_t error = 0;
    cv_val += 524288;
    uint32_t truncated_cv_val = (cv_val - error) & 0xFFFFFF00;
    error += truncated_cv_val - cv_val;
    int16_t val = int32_t(truncated_cv_val >> 8) - 2048;
    if (Connected(Input::Audio2)) {
      LedOn(1, true);
      AudioOut2((val * AudioIn2()) >> 12);
    } else {
      LedOn(1, false);
      AudioOut2(val);
    }
  }
  void SetCV1(int32_t cv_val) {
    static int32_t error = 0;
    cv_val += 524288;
    uint32_t truncated_cv_val = (cv_val - error) & 0xFFFFFF00;
    error += truncated_cv_val - cv_val;
    int16_t val = 2048 - int32_t(truncated_cv_val >> 8);
    if (Connected(Input::CV1)) {
      LedOn(2, true);
      CVOut1((val * CVIn1()) >> 12);
    } else {
      LedOn(2, false);
      CVOut1(val);
    }
  }
  void SetCV2(int32_t cv_val) {
    static int32_t error = 0;
    cv_val += 524288;
    uint32_t truncated_cv_val = (cv_val - error) & 0xFFFFFF00;
    error += truncated_cv_val - cv_val;
    int16_t val = int32_t(truncated_cv_val >> 8) - 2048;
    if (Connected(Input::CV2)) {
      LedOn(3, true);
      CVOut2((val * CVIn2()) >> 12);
    } else {
      LedOn(3, false);
      CVOut2(val);
    }
  }
  virtual void __not_in_flash_func(ProcessSample)() {
    int32_t omega = ExpVoct(KnobVal(Knob::Main)) >> 12;
    int32_t diff;
    diff = (SwitchVal() == Switch::Middle) ? 31 : 1493;
    if (SwitchVal() == Switch::Down) { reset(); }

    //if (diff>omega) diff=omega;
    phase1 += omega;
    phase2 += (omega >> 1) + diff;
    phase3 += (omega >> 2) + (diff * 3) + (diff >> 2);
    phase4 += (omega >> 3) + (diff * 4) + (diff >> 3);

    if (PulseIn1RisingEdge()) { reset(); }

    SetAudio1(sinval(phase1 >> 11) >> 3);
    SetAudio2(sinval(phase2 >> 11) >> 3);
    SetCV1(sinval(phase3 >> 11) >> 3);
    SetCV2(sinval(phase4 >> 11) >> 3);
  }
};

SlowMod sm;

void setup() {
  sm.EnableNormalisationProbe();
  sm.Run();
}

void loop() {
}
