<?php

$labels['filters'] = '필터';
$labels['managefilters'] = '받는 메일 필터 관리';
$labels['filtername'] = '필터 이름';
$labels['newfilter'] = '새로운 필터';
$labels['filteradd'] = '필터 추가';
$labels['filterdel'] = '필터 삭제';
$labels['moveup'] = '위로 이동';
$labels['movedown'] = '아래로 이동';
$labels['filterallof'] = '다음의 모든 규칙과 일치';
$labels['filteranyof'] = '다음의 일부 규칙과 일치';
$labels['filterany'] = '모든 메시지';
$labels['filtercontains'] = '다음을 포함';
$labels['filternotcontains'] = '다음을 포함하지 않음';
$labels['filteris'] = '다음과 같음';
$labels['filterisnot'] = '다음과 같지 않음';
$labels['filterexists'] = '존재함';
$labels['filternotexists'] = '존재하지 않음';
$labels['filterunder'] = '미만';
$labels['filterover'] = '초과';
$labels['addrule'] = '규칙 추가';
$labels['delrule'] = '규칙 삭제';
$labels['messagemoveto'] = '다음으로 메시지 이동';
$labels['messageredirect'] = '다음으로 메시지 전달';
$labels['messagecopyto'] = '다음으로 메시지 복사';
$labels['messagesendcopy'] = '다음으로 메시지 복사본 전송';
$labels['messagereply'] = '다음 메시지로 답장';
$labels['messagedelete'] = '메시지 삭제';
$labels['messagediscard'] = '다음 메시지 보내고 지우기';
$labels['messagesrules'] = '받는 메일:';
$labels['messagesactions'] = '...다음 동작 실행:';
$labels['add'] = '추가';
$labels['del'] = '삭제';
$labels['sender'] = '보낸 사람';
$labels['recipient'] = '받는 사람';
$labels['vacationaddresses'] = '추가로 받을 사람의 이메일 목록(쉼표로 구분):';
$labels['vacationdays'] = '메시지 전송 간격(일 단위):';
$labels['vacationreason'] = '메시지 본문(휴가 중):';
$labels['rulestop'] = '규칙 평가 중단';
$labels['filterset'] = '필터 세트';
$labels['filtersetadd'] = '필터 세트 추가';
$labels['filtersetdel'] = '현재 필터 세트 삭제';
$labels['filtersetact'] = '현재 필터 세트 활성화';
$labels['filtersetdeact'] = '현재 필터 세트 비활성화';
$labels['filtersetget'] = '텍스트 포맷으로 필터 세트 다운로드';
$labels['filterdef'] = '필터 정의';
$labels['filtersetname'] = '필터 세트 이름';
$labels['newfilterset'] = '새로운 필터 세트';
$labels['active'] = '활성화됨';
$labels['none'] = '없음';
$labels['fromset'] = '세트로부터';
$labels['fromfile'] = '파일로부터';
$labels['filterdisabled'] = '필터 비활성화됨';
$labels['countisgreaterthan'] = '수가 다음보다 큼';
$labels['countisgreaterthanequal'] = '수가 다음보다 크거나 같음';
$labels['countislessthan'] = '수가 다음보다 작음';
$labels['countislessthanequal'] = '수가 다음보다 작거나 같음';
$labels['countequals'] = '수가 다음과 같음';
$labels['countnotequals'] = '수가 다음과 같지 않음';
$labels['valueisgreaterthan'] = '값이 다음보다 큼';
$labels['valueisgreaterthanequal'] = '값이 다음보다 크거나 같음';
$labels['valueislessthan'] = '값이 다음보다 작음';
$labels['valueislessthanequal'] = '값이 다음보다 작거나 같음';
$labels['valueequals'] = '값이 다음과 같음';
$labels['valuenotequals'] = '값이 다음과 같지 않음';

$messages = array();
$messages['filterunknownerror'] = '알 수 없는 서버 오류';
$messages['filterconnerror'] = 'managesieve 서버에 연결할 수 없음';
$messages['filterdeleteerror'] = '필터 삭제 안 됨. 서버 오류 발생함';
$messages['filterdeleted'] = '성공적으로 필터 삭제됨';
$messages['filtersaved'] = '성공적으로 필터 저장됨';
$messages['filtersaveerror'] = '필터를 저장할 수 없음. 서버 오류 발생함';
$messages['filterdeleteconfirm'] = '선택한 필터를 삭제하겠습니까?';
$messages['ruledeleteconfirm'] = '선택한 규칙을 삭제하겠습니까?';
$messages['actiondeleteconfirm'] = '선택한 동작을 삭제하겠습니까?';
$messages['forbiddenchars'] = '필드에 금지된 문자 있음';
$messages['cannotbeempty'] = '필드를 공백으로 남겨둘 수 없음';
$messages['setactivateerror'] = '선택한 필터 세트를 활성화할 수 없음. 서버 오류 발생함';
$messages['setdeactivateerror'] = '선택한 필터 세트를 비활성화할 수 없음. 서버 오류 발생함';
$messages['setdeleteerror'] = '선택한 필터 세트를 삭제할 수 없음. 서버 오류 발생함';
$messages['setactivated'] = '성공적으로 필터 세트 활성화됨';
$messages['setdeactivated'] = '성공적으로 필터 세트 비활성화됨';
$messages['setdeleted'] = '성공적으로 필터 세트 삭제됨';
$messages['setdeleteconfirm'] = '선택한 필터 세트를 삭제하겠습니까?';
$messages['setcreateerror'] = '필터 세트를 생성할 수 없음. 서버 오류 발생함';
$messages['setcreated'] = '성공적으로 필터 세트 생성됨';
$messages['emptyname'] = '필터 세트를 생성할 수 없음. 비어 있는 세트 이름임';
$messages['nametoolong'] = '필터 세트를 생성할 수 없음. 너무 긴 이름임'

?>
