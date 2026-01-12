## 결과 화면 (Screenshots)

<details open>
<summary><b>기하 변환</b></summary>

<table>
  <tr>
    <td align="center"><b>회전(각도 40°)</b><br/><img src="docs/images/rotate_deg40.png" width="420"/></td>
    <td align="center"><b>대칭(좌우 / 상하)</b><br/><img src="docs/images/flip_lr_ud.png" width="420"/></td>
  </tr>
  <tr>
    <td align="center"><b>이동 변환</b><br/><img src="docs/images/translation.png" width="420"/></td>
    <td align="center"><b>크기 변환(리사이즈)</b><br/><img src="docs/images/scaling_resize.png" width="420"/></td>
  </tr>
</table>

<ul>
  <li><b>좌우 대칭</b>: (원본, 좌우)</li>
  <li><b>상하 대칭</b>: (원본, 상하)</li>
  <li><b>flip_lr_ud.png</b> 배치
    <ul>
      <li>좌측 상단: 원본</li>
      <li>우측 상단: 좌우대칭</li>
      <li>좌측 하단: 상하대칭</li>
    </ul>
  </li>
</ul>

</details>

<details>
<summary><b>잡음(Noise) 추가</b></summary>

<img src="docs/images/noise_sp25_gaussian25.png" width="900"/>

<ul>
  <li>우측 상단: <b>소금&amp;후추 잡음 25%</b></li>
  <li>좌측 하단: <b>가우시안 잡음 25%</b></li>
</ul>

</details>

<details>
<summary><b>공간 영역 필터(Spatial Filters)</b></summary>

<table>
  <tr>
    <td align="center"><b>평균값 필터</b><br/><img src="docs/images/filter_mean.png" width="420"/></td>
    <td align="center"><b>가중 평균값 필터</b><br/><img src="docs/images/filter_weighted_mean.png" width="420"/></td>
  </tr>
  <tr>
    <td align="center"><b>가우시안 필터 (sigma=2)</b><br/><img src="docs/images/filter_gaussian_sigma2.png" width="420"/></td>
    <td align="center"><b>미디언 필터</b><br/><img src="docs/images/filter_median.png" width="420"/></td>
  </tr>
  <tr>
    <td align="center"><b>라플라시안 필터</b><br/><img src="docs/images/filter_laplacian.png" width="420"/></td>
    <td align="center"><b>언샤프(샤프닝)</b><br/><img src="docs/images/filter_unsharp.png" width="420"/></td>
  </tr>
  <tr>
    <td align="center"><b>하이부스트 필터</b><br/><img src="docs/images/filter_highboost.png" width="420"/></td>
    <td align="center"><b>비등방성(Anisotropic) 필터</b><br/>
      <img src="docs/images/anisotropic_diffusion_lambda0.2_k10_iter10.png" width="420"/>
      <br/><sub>lambda=0.2, K=10, 반복=10</sub>
    </td>
  </tr>
</table>

</details>

<details>
<summary><b>푸리에 변환(주파수 영역) &amp; 주파수 필터링</b></summary>

<table>
  <tr>
    <td align="center"><b>푸리에 변환 화면</b><br/><img src="docs/images/fft_views.png" width="420"/></td>
    <td align="center"><b>주파수 영역 필터(차단 주파수=32)</b><br/><img src="docs/images/freq_filters_fc32.png" width="420"/></td>
  </tr>
</table>

<ul>
  <li><b>fft_views.png</b> 배치
    <ul>
      <li>좌측 상단: <b>스펙트럼 영역</b></li>
      <li>좌측 하단: <b>주파수 영역</b></li>
    </ul>
  </li>

  <li><b>freq_filters_fc32.png</b> 배치 (차단 주파수 32)
    <ul>
      <li>좌측 상단: <b>이상적 고역통과(HPF)</b></li>
      <li>우측 상단: <b>가우시안 고역통과(HPF)</b></li>
      <li>좌측 하단: <b>이상적 저역통과(LPF)</b></li>
      <li>우측 하단: <b>가우시안 저역통과(LPF)</b></li>
    </ul>
  </li>
</ul>

</details>

<details>
<summary><b>엣지(Edge) 검출</b></summary>

<img src="docs/images/edge_roberts_prewitt_sobel_canny.png" width="900"/>

<ul>
  <li>로버츠(Roberts) / 프리윗(Prewitt) / 소벨(Sobel) / 캐니(Canny) 결과 비교</li>
  <li><b>캐니 파라미터</b>: sigma = <b>1.4</b>, low threshold = <b>340</b>, high threshold = <b>60</b></li>
</ul>

</details>

<details>
<summary><b>특징(Feature) 검출</b></summary>

<table>
  <tr>
    <td align="center"><b>허프 변환 직선 검출</b><br/><img src="docs/images/hough_lines.png" width="420"/></td>
    <td align="center"><b>해리스 코너 검출</b><br/><img src="docs/images/harris_corners.png" width="420"/></td>
  </tr>
</table>

</details>

<details>
<summary><b>비트 평면 / 산술·논리 연산 / 이진화·모폴로지</b></summary>

<table>
  <tr>
    <td align="center"><b>비트 평면 나누기</b><br/><img src="docs/images/bitplane_slicing.png" width="420"/></td>
    <td align="center"><b>두 영상 산술·논리 연산(UI)</b><br/><img src="docs/images/arithmetic_bitwise_ops_ui.png" width="420"/></td>
  </tr>
  <tr>
    <td align="center" colspan="2"><b>산술·논리 연산 결과</b><br/>
      <img src="docs/images/arithmetic_bitwise_ops_result.png" width="900"/>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td align="center"><b>이진화</b><br/><img src="docs/images/binary_threshold_134.png" width="420"/>
      <br/><sub>임계값(threshold) = 134</sub>
    </td>
    <td align="center"><b>모폴로지 연산</b><br/><img src="docs/images/morphology_erode_dilate_open_close.png" width="420"/></td>
  </tr>
</table>

<ul>
  <li><b>morphology_erode_dilate_open_close.png</b> 배치
    <ul>
      <li>왼쪽: <b>이진화 → 침식(Erosion) → 팽창(Dilation)</b></li>
      <li>오른쪽: <b>원본 → 열기(Opening) → 닫기(Closing)</b></li>
    </ul>
  </li>
</ul>

</details>

<details>
<summary><b>히스토그램 / 평활화</b></summary>

<img src="docs/images/histogram_equalization.png" width="900"/>

<ul>
  <li>그레이/컬러 영상의 히스토그램(RGB 채널별) 및 평활화 결과 비교</li>
</ul>

</details>
